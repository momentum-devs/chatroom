#include "LoginState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
LoginState::LoginState(std::unique_ptr<LoginController> loginControllerInit,
                       std::shared_ptr<LoaderController> loaderControllerInit)
    : loginController{std::move(loginControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void LoginState::activate()
{
    LOG_S(INFO) << "Activate LoginState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, loginController.get());

    loaderController->callLoadView(qUrl);

    loginController->activate();
}

void LoginState::deactivate()
{
    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    loginController->deactivate();
}
}