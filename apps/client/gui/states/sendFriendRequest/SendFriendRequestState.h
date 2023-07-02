#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "SendFriendRequestController.h"

namespace client::gui
{
class SendFriendRequestState : public State
{
public:
    SendFriendRequestState(std::unique_ptr<SendFriendRequestController> sendFriendRequestController,
                           std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"sendFriendRequestController"};
    inline static const QUrl qUrl{
        QUrl::fromLocalFile("chatroom/gui/states/sendFriendRequest/SendFriendRequestView.qml")};
    std::unique_ptr<SendFriendRequestController> sendFriendRequestController;
    std::shared_ptr<LoaderController> loaderController;
};
}
