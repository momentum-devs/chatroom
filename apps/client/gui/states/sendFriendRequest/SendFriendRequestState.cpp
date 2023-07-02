#include "SendFriendRequestState.h"

#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
SendFriendRequestState::SendFriendRequestState(
    std::unique_ptr<SendFriendRequestController> sendFriendRequestControllerInit,
    std::shared_ptr<LoaderController> loaderControllerInit)
    : sendFriendRequestController{std::move(sendFriendRequestControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void SendFriendRequestState::activate()
{
    LOG_S(INFO) << "Activate SendFriendRequestState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, sendFriendRequestController.get());

    loaderController->callLoadView(qUrl);

    sendFriendRequestController->activate();
}

void SendFriendRequestState::deactivate()
{
    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    sendFriendRequestController->deactivate();
}
}