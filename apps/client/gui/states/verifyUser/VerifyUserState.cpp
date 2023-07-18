
#include "VerifyUserState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
VerifyUserState::VerifyUserState(std::unique_ptr<VerifyUserController> verifyUserControllerInit,
                                 std::shared_ptr<LoaderController> loaderControllerInit)
    : verifyUserController{std::move(verifyUserControllerInit)}, loaderController{std::move(loaderControllerInit)}
{
}

void VerifyUserState::activate() {}

void VerifyUserState::deactivate() {}
}