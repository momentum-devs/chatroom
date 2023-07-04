#include "UserSettingsState.h"

#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
UserSettingsState::UserSettingsState(std::unique_ptr<UserSettingsController> userSettingsControllerInit,
                                     std::shared_ptr<LoaderController> loaderControllerInit)
    : userSettingsController{std::move(userSettingsControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void UserSettingsState::activate()
{
    LOG_S(INFO) << "Activate UserSettingsState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, userSettingsController.get());

    loaderController->callLoadView(qUrl);

    userSettingsController->activate();
}

void UserSettingsState::deactivate()
{
    userSettingsController->deactivate();
}
}