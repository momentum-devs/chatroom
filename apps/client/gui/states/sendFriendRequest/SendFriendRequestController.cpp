#include "SendFriendRequestController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
SendFriendRequestController::SendFriendRequestController(std::shared_ptr<api::Session> sessionInit,
                                                         const StateFactory& stateFactoryInit,
                                                         std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void SendFriendRequestController::activate() {}

void SendFriendRequestController::deactivate() {}

void SendFriendRequestController::sendFriendRequest(const QString& friendEmail) {}

void SendFriendRequestController::goBack()
{
    stateMachine->returnToThePreviousState();
}
}