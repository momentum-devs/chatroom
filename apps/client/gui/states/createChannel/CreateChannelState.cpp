
#include "CreateChannelState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
CreateChannelState::CreateChannelState(std::unique_ptr<CreateChannelController> createChannelControllerInit,
                                       std::shared_ptr<LoaderController> loaderControllerInit)
    : createChannelController{std::move(createChannelControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void CreateChannelState::activate()
{
    LOG_S(INFO) << "Activate CreateChannelState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, createChannelController.get());

    loaderController->callLoadView(qUrl);

    createChannelController->activate();
}

void CreateChannelState::deactivate() {}
}