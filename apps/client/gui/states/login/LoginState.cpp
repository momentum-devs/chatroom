#include "LoginState.h"

#include <glog/logging.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
LoginState::LoginState(std::unique_ptr<LoginController> loginControllerInit,
                       std::shared_ptr<LoaderController> loaderControllerInit)
    : loginController{std::move(loginControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void LoginState::activate()
{
    VLOG(0) << "Load LoginState";

    QObject::connect(loginController.get(), &LoginController::loginRequest, loginController.get(),
                     &LoginController::handleLoginRequest);

    QObject::connect(loginController.get(), &LoginController::goToRegisterState, loginController.get(),
                     &LoginController::handleGoToRegisterState);

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, loginController.get());

    loaderController->callLoadView(qUrl);

    loginController->activate();
}

void LoginState::deactivate()
{
    QObject::disconnect(loginController.get(), &LoginController::loginRequest, loginController.get(),
                        &LoginController::handleLoginRequest);

    QObject::disconnect(loginController.get(), &LoginController::goToRegisterState, loginController.get(),
                        &LoginController::handleGoToRegisterState);

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    loginController->deactivate();
}
}