#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "InviteToChannelController.h"

namespace client::gui
{
class InviteToChannelState : public State
{
public:
    InviteToChannelState(std::unique_ptr<InviteToChannelController> registerController,
                         std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"inviteToChannelController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/inviteToChannel/InviteToChannelView.qml")};
    std::unique_ptr<InviteToChannelController> inviteToChannelController;
    std::shared_ptr<LoaderController> loaderController;
};
}
