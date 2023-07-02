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

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, registerController.get());

    loaderController->callLoadView(qUrl);

    registerController->activate();
}

void RegisterState::deactivate()
{
    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    registerController->deactivate();
}
}