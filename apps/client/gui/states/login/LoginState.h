#pragma once

#include <QQuickView>

#include "client/gui/states/State.h"
#include "client/gui/states/StateMachine.h"
#include "LoginController.h"

namespace client::gui
{
class LoginState : public State
{
public:
    LoginState(std::unique_ptr<LoginController> registerController, std::shared_ptr<LoaderController> loaderController);
    void activate() override;
    void deactivate() override;

private:
    inline static const QString componentName{"loginController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/states/login/LoginView.qml")};
    std::unique_ptr<LoginController> loginController;
    std::shared_ptr<LoaderController> loaderController;
};
}