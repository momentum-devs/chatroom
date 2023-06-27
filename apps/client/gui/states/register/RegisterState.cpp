#include "RegisterState.h"

#include <glog/logging.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
RegisterState::RegisterState(std::unique_ptr<RegisterController> registerControllerInit,
                             std::shared_ptr<LoaderController> loaderControllerInit)
    : registerController{std::move(registerControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void RegisterState::activate()
{
    VLOG(0) << "Load RegisterState";

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