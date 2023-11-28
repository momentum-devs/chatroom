#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "SendFriendInvitationController.h"

namespace client::gui
{
class SendFriendInvitationState : public State
{
public:
    SendFriendInvitationState(std::unique_ptr<SendFriendInvitationController> sendFriendInvitationController,
                              std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"sendFriendInvitationController"};
    inline static const QUrl qUrl{
        QUrl::fromLocalFile("chatroom/gui/states/sendFriendInvitation/SendFriendInvitationView.qml")};
    std::unique_ptr<SendFriendInvitationController> sendFriendInvitationController;
    std::shared_ptr<LoaderController> loaderController;
};
}
