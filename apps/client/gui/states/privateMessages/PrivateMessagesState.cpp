#include "PrivateMessagesState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "loguru.hpp"

namespace client::gui
{
PrivateMessagesState::PrivateMessagesState(std::unique_ptr<LeftColumnController> leftColumnControllerInit,
                                           std::shared_ptr<LoaderController> loaderControllerInit)
    : leftColumnController{std::move(leftColumnControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void PrivateMessagesState::activate()
{
    LOG_S(INFO) << "Activate MainState";

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(),
                                                                     leftColumnController.get());

    loaderController->callLoadView(qUrl, [this]() { leftColumnController->activate(); });
}

void PrivateMessagesState::deactivate()
{
    LOG_S(INFO) << "Deactivate MainState";

    loaderController->getEngine()->rootContext()->setContextProperty(leftColumnController->getName(), nullptr);

    leftColumnController->deactivate();
}
}