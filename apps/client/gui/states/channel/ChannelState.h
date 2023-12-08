#pragma once

#include "ChannelController.h"
#include "client/gui/controllers/LeftColumnController.h"
#include "client/gui/controllers/MessagesController.h"
#include "client/gui/qml/LoaderController.h"
#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class ChannelState : public State
{
public:
    ChannelState(std::unique_ptr<ChannelController> channelController,
                 std::unique_ptr<LeftColumnController> leftColumnController,
                 std::unique_ptr<MessagesController> messagesController,
                 std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/channel/ChannelView.qml")};
    std::unique_ptr<ChannelController> channelController;
    std::unique_ptr<LeftColumnController> leftColumnController;
    std::unique_ptr<MessagesController> messagesController;
    std::shared_ptr<LoaderController> loaderController;
};
}