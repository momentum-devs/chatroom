#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "RegisterController.h"

namespace client::gui
{
class RegisterState : public State
{
public:
    RegisterState(std::unique_ptr<RegisterController> registerController,
                  std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"registerController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/register/RegisterView.qml")};
    std::unique_ptr<RegisterController> registerController;
    std::shared_ptr<LoaderController> loaderController;
};
}