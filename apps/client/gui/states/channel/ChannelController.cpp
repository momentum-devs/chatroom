#include "ChannelController.h"

#include "loguru.hpp"

namespace client::gui
{
ChannelController::ChannelController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                     std::shared_ptr<StateMachine> stateMachineInit,
                                     const std::string& initialChannelId)
    : session{std::move(sessionInit)},
      stateFactory{std::move(stateFactoryInit)},
      stateMachine{std::move(stateMachineInit)},
      currentChannelId{initialChannelId}
{
}

void ChannelController::activate() {}

void ChannelController::deactivate() {}

const QString& ChannelController::getName() const
{
    return name;
}

void ChannelController::goToChannel(const QString& channelId)
{
    LOG_S(INFO) << std::format("Set channel wit id {} as current", channelId.toStdString());

    // TODO: implement
}

void ChannelController::goToPrivateMessages()
{
    LOG_S(INFO) << "Go to private messages";

    stateMachine->returnToThePreviousState();
}
}