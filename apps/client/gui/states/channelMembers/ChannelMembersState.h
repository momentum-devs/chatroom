#pragma once

#include <QQuickView>

#include "ChannelMembersController.h"
#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class ChannelMembersState : public State
{
public:
    ChannelMembersState(std::unique_ptr<ChannelMembersController> registerController,
                        std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"channelMembersController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/channelMembers/ChannelMembersView.qml")};
    std::unique_ptr<ChannelMembersController> channelMembersController;
    std::shared_ptr<LoaderController> loaderController;
};
}
