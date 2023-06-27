#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "CreateChannelController.h"

namespace client::gui
{
class CreateChannelState : public State
{
public:
    CreateChannelState(std::unique_ptr<CreateChannelController> registerController,
                       std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"createChannelController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/createChannel/CreateChannelView.qml")};
    std::unique_ptr<CreateChannelController> createChannelController;
    std::shared_ptr<LoaderController> loaderController;
};
}
