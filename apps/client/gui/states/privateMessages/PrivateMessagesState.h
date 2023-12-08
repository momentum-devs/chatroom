#pragma once

#include "client/gui/controllers/LeftColumnController.h"
#include "client/gui/controllers/MessagesController.h"
#include "client/gui/qml/LoaderController.h"
#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "PrivateMessagesController.h"

namespace client::gui
{
class PrivateMessagesState : public State
{
public:
    PrivateMessagesState(std::unique_ptr<PrivateMessagesController> privateMessagesController,
                         std::unique_ptr<LeftColumnController> leftColumnController,
                         std::unique_ptr<MessagesController> messagesController,
                         std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/privateMessages/PrivateMessagesView.qml")};
    std::unique_ptr<PrivateMessagesController> privateMessagesController;
    std::unique_ptr<LeftColumnController> leftColumnController;
    std::unique_ptr<MessagesController> messagesController;
    std::shared_ptr<LoaderController> loaderController;
};
}