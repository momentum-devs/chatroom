#include "VerifyUserState.h"

#include <loguru.hpp>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
VerifyUserState::VerifyUserState(std::unique_ptr<VerifyUserController> verifyUserControllerInit,
                                 std::shared_ptr<LoaderController> loaderControllerInit)
    : verifyUserController{std::move(verifyUserControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void VerifyUserState::activate()
{
    LOG_S(INFO) << "Activate VerifyUserState";

    loaderController->getEngine()->rootContext()->setContextProperty(componentName, verifyUserController.get());

    loaderController->callLoadView(qUrl);

    verifyUserController->activate();
}

void VerifyUserState::deactivate()
{
    verifyUserController->deactivate();
}
}