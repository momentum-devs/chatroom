#include "StateFactory.h"

#include "register/RegisterController.h"
#include "register/RegisterState.h"

namespace client::gui
{
StateFactory::StateFactory(std::shared_ptr<client::api::Session> sessionInit,
                           std::shared_ptr<StateMachine> stateMachineInit, std::shared_ptr<QQuickView> viewInit)
    : session{std::move(sessionInit)}, stateMachine{std::move(stateMachineInit)}, view{std::move(viewInit)}
{
}

std::shared_ptr<State> StateFactory::createDefaultState() const
{
    return createRegisterState();
}

std::shared_ptr<State> StateFactory::createRegisterState() const
{
    auto controller = std::make_unique<RegisterController>(session, *this, stateMachine);

    return std::make_shared<RegisterState>(std::move(controller), view);
}
}