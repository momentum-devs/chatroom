#include "UserSettingsController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
UserSettingsController::UserSettingsController(std::shared_ptr<api::Session> sessionInit,
                                               const StateFactory& stateFactoryInit,
                                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void UserSettingsController::activate() {}

void UserSettingsController::deactivate() {}

void UserSettingsController::changeNickname(const QString& nickname) {}

void UserSettingsController::changePassword(const QString& password) {}

void UserSettingsController::deleteUser() {}

void UserSettingsController::goBack()
{
    stateMachine->returnToThePreviousState();
}
}