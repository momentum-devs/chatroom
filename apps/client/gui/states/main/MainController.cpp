#include "MainController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
MainController::MainController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void MainController::activate()
{
    // TODO: implement
}

void MainController::deactivate()
{
    // TODO: implement
}
}