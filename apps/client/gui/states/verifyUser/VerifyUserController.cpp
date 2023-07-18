#include "VerifyUserController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
VerifyUserController::VerifyUserController(std::shared_ptr<api::Session> sessionInit,
                                           const StateFactory& stateFactoryInit,
                                           std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void VerifyUserController::activate() {}

void VerifyUserController::deactivate() {}
}