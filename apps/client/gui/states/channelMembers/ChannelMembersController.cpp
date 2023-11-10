#include "ChannelMembersController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
ChannelMembersController::ChannelMembersController(std::shared_ptr<api::Session> sessionInit,
                                                   const StateFactory& stateFactoryInit,
                                                   std::shared_ptr<StateMachine> stateMachineInit,
                                                   const std::string& channelIdInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      channelId{channelIdInit}
{
}

void ChannelMembersController::activate() {}

void ChannelMembersController::deactivate() {}

void ChannelMembersController::goBack()
{
    LOG_S(INFO) << "Return to previous state";

    stateMachine->returnToThePreviousState();
}
}