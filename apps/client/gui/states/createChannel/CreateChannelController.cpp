#include "CreateChannelController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
CreateChannelController::CreateChannelController(std::shared_ptr<api::Session> sessionInit,
                                                 const StateFactory& stateFactoryInit,
                                                 std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void CreateChannelController::activate() {}

void CreateChannelController::deactivate() {}
}