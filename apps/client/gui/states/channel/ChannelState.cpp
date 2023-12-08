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

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(channelController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(messagesController->getName(), nullptr);

    leftColumnController->deactivate();

    channelController->deactivate();
}

}