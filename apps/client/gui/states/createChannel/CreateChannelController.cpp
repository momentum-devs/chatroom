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

void CreateChannelController::goBack()
{
    LOG_S(INFO) << "Return to previous state";
    stateMachine->returnToThePreviousState();
}

void CreateChannelController::createChannel(const QString& channelName)
{
    LOG_S(INFO) << "Create channel with name: " << channelName.toStdString();
}
}