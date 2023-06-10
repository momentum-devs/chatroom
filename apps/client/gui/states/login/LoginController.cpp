#include "LoginController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
LoginController::LoginController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                 std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void LoginController::handleLoginRequest(const QString& email, const QString& password)
{
    // TODO: send login message
}

void LoginController::handleGoToRegisterState()
{
    stateMachine->addNextState(stateFactory.createRegisterState());
}
}