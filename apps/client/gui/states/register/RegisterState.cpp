#include "RegisterState.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
RegisterState::RegisterState(std::unique_ptr<RegisterController> registerControllerInit,
                             std::shared_ptr<QQuickView> viewInit)
    : registerController{std::move(registerControllerInit)}, view{std::move(viewInit)}
{
}

void RegisterState::activate()
{
    QObject::connect(registerController.get(), &RegisterController::registerRequest, registerController.get(),
                     &RegisterController::handleRegisterRequest);

    view->engine()->rootContext()->setContextProperty(componentName, registerController.get());

    view->setSource(qUrl);

    view->show();
}

void RegisterState::deactivate()
{
    QObject::disconnect(registerController.get(), &RegisterController::registerRequest, registerController.get(),
                        &RegisterController::handleRegisterRequest);
}
}