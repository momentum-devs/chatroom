#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "SendFriendInvitationState.h"

namespace client::gui
{
SendFriendInvitationState::SendFriendInvitationState(
    std::unique_ptr<SendFriendInvitationController> sendFriendInvitationControllerInit,
    std::shared_ptr<LoaderController> loaderControllerInit)
    : sendFriendInvitationController{std::move(sendFriendInvitationControllerInit)},
      loaderController{std::move(loaderControllerInit)}
{
}

void SendFriendInvitationState::activate()
{
    LOG_S(INFO) << "Activate SendFriendInvitationState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName,
                                                                     sendFriendInvitationController.get());

    loaderController->callLoadView(qUrl);

    sendFriendInvitationController->activate();
}

void SendFriendInvitationState::deactivate()
{
    LOG_S(INFO) << "Deactivate SendFriendInvitationState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    sendFriendInvitationController->deactivate();
}
}