#include "InviteToChannelController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
InviteToChannelController::InviteToChannelController(std::shared_ptr<api::Session> sessionInit,
                                                     const StateFactory& stateFactoryInit,
                                                     std::shared_ptr<StateMachine> stateMachineInit,
                                                     const std::string& channelIdInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      channelId{channelIdInit}
{
}

void InviteToChannelController::activate() {}

void InviteToChannelController::deactivate() {}
}