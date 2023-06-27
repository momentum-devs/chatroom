#include "MainState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
MainState::MainState(std::unique_ptr<MainController> mainControllerInit,
                     std::shared_ptr<LoaderController> loaderControllerInit)
    : mainController{std::move(mainControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void MainState::activate()
{
    LOG_S(INFO) << "Activate MainState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, mainController.get());

    loaderController->callLoadView(qUrl);

    mainController->activate();
}

void MainState::deactivate()
{
    LOG_S(INFO) << "Deactivate MainState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, nullptr);

    mainController->deactivate();
}
}