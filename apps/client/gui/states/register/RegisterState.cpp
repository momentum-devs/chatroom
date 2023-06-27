#include "RegisterState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
RegisterState::RegisterState(std::unique_ptr<RegisterController> registerControllerInit,
                             std::shared_ptr<LoaderController> loaderControllerInit)
    : registerController{std::move(registerControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void RegisterState::activate()
{
    LOG_S(INFO) << "Activate RegisterState";

    QObject::connect(registerController.get(), &RegisterController::registerRequest, registerController.get(),
                     &RegisterController::handleRegisterRequest);

    QObject::connect(registerController.get(), &RegisterController::goBack, registerController.get(),
                     &RegisterController::handleGoBack);

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, registerController.get());

    loaderController->callLoadView(qUrl);

    registerController->activate();
}

void RegisterState::deactivate()
{
    QObject::disconnect(registerController.get(), &RegisterController::registerRequest, registerController.get(),
                        &RegisterController::handleRegisterRequest);

    QObject::disconnect(registerController.get(), &RegisterController::goBack, registerController.get(),
                        &RegisterController::handleGoBack);

    registerController->deactivate();
}
}