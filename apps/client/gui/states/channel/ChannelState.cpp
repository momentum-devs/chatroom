#include "ChannelState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
ChannelState::ChannelState(std::unique_ptr<ChannelController> channelControllerInit,
                           std::unique_ptr<LeftColumnController> leftColumnControllerInit,
                           std::unique_ptr<MessagesController> messagesControllerInit,
                           std::shared_ptr<LoaderController> loaderControllerInit)
    : channelController{std::move(channelControllerInit)},
      leftColumnController{std::move(leftColumnControllerInit)},
      messagesController{std::move(messagesControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void ChannelState::activate()
{
    LOG_S(INFO) << "Activate ChannelState";

    QObject::connect(leftColumnController.get(), &LeftColumnController::goToChannelSignal, channelController.get(),
                     &ChannelController::goToChannel);

    QObject::connect(leftColumnController.get(), &LeftColumnController::goToPrivateMessagesSignal,
                     channelController.get(), &ChannelController::goToPrivateMessages);

    QObject::connect(messagesController.get(), &MessagesController::newMessageToSend, channelController.get(),
                     &ChannelController::sendChannelMessage);

    QObject::connect(channelController.get(), &ChannelController::messagesUpdated, messagesController.get(),
                     &MessagesController::handleMessageUpdate);

    QObject::connect(channelController.get(), &ChannelController::setMessageStorage, messagesController.get(),
                     &MessagesController::setMessageStorage);

    QObject::connect(messagesController.get(), &MessagesController::getMoreMessages, channelController.get(),
                     &ChannelController::getMoreMessages);

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(),
                                                                     leftColumnController.get());

    loaderController->getEngine()->rootContext()->setContextProperty(channelController->getName(),
                                                                     channelController.get());

    loaderController->getEngine()->rootContext()->setContextProperty(messagesController->getName(),
                                                                     messagesController.get());

    loaderController->callLoadView(qUrl,
                                   [this]()
                                   {
                                       leftColumnController->activate();
                                       channelController->activate();
                                   });
}

void ChannelState::deactivate()
{
    LOG_S(INFO) << "Deactivate ChannelState";

    QObject::disconnect(leftColumnController.get(), &LeftColumnController::goToChannelSignal, channelController.get(),
                        &ChannelController::goToChannel);

    QObject::disconnect(leftColumnController.get(), &LeftColumnController::goToPrivateMessagesSignal,
                        channelController.get(), &ChannelController::goToPrivateMessages);

    QObject::disconnect(messagesController.get(), &MessagesController::newMessageToSend, channelController.get(),
                        &ChannelController::sendChannelMessage);

    QObject::disconnect(channelController.get(), &ChannelController::messagesUpdated, messagesController.get(),
                        &MessagesController::handleMessageUpdate);

    QObject::disconnect(channelController.get(), &ChannelController::setMessageStorage, messagesController.get(),
                        &MessagesController::setMessageStorage);

    QObject::disconnect(messagesController.get(), &MessagesController::getMoreMessages, channelController.get(),
                        &ChannelController::getMoreMessages);

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(channelController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(messagesController->getName(), nullptr);

    leftColumnController->deactivate();

    channelController->deactivate();
}
}
