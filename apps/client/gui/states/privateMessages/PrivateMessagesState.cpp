#include "PrivateMessagesState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
PrivateMessagesState::PrivateMessagesState(std::unique_ptr<PrivateMessagesController> privateMessagesControllerInit,
                                           std::unique_ptr<LeftColumnController> leftColumnControllerInit,
                                           std::unique_ptr<MessagesController> messagesControllerInit,
                                           std::shared_ptr<LoaderController> loaderControllerInit)
    : privateMessagesController{std::move(privateMessagesControllerInit)},
      leftColumnController{std::move(leftColumnControllerInit)},
      messagesController{std::move(messagesControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void PrivateMessagesState::activate()
{
    LOG_S(INFO) << "Activate PrivateMessagesState";

    QObject::connect(leftColumnController.get(), &LeftColumnController::goToChannelSignal,
                     privateMessagesController.get(), &PrivateMessagesController::goToChannel);

    QObject::connect(privateMessagesController.get(), &PrivateMessagesController::setMessageStorage,
                     messagesController.get(), &MessagesController::setMessageStorage);

    QObject::connect(messagesController.get(), &MessagesController::newMessageToSend, privateMessagesController.get(),
                     &PrivateMessagesController::sendPrivateMessage);

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(),
                                                                     leftColumnController.get());

    loaderController->getEngine()->rootContext()->setContextProperty(privateMessagesController->getName(),
                                                                     privateMessagesController.get());

    loaderController->getEngine()->rootContext()->setContextProperty(messagesController->getName(),
                                                                     messagesController.get());

    loaderController->callLoadView(qUrl,
                                   [this]()
                                   {
                                       leftColumnController->activate();
                                       privateMessagesController->activate();
                                   });
}

void PrivateMessagesState::deactivate()
{
    LOG_S(INFO) << "Deactivate PrivateMessagesState";

    QObject::disconnect(leftColumnController.get(), &LeftColumnController::goToChannelSignal,
                        privateMessagesController.get(), &PrivateMessagesController::goToChannel);

    QObject::disconnect(privateMessagesController.get(), &PrivateMessagesController::setMessageStorage,
                        messagesController.get(), &MessagesController::setMessageStorage);

    QObject::disconnect(messagesController.get(), &MessagesController::newMessageToSend,
                        privateMessagesController.get(), &PrivateMessagesController::sendPrivateMessage);

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(privateMessagesController->getName(), nullptr);

    loaderController->getEngine()->rootContext()->setContextProperty(messagesController->getName(), nullptr);

    leftColumnController->deactivate();

    privateMessagesController->deactivate();
}
}