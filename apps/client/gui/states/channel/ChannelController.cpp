#include "ChannelController.h"

#include "loguru.hpp"

namespace client::gui
{
ChannelController::ChannelController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                     std::shared_ptr<StateMachine> stateMachineInit,
                                     const std::string& initialChannelId, const std::string& initialChannelName,
                                     bool initialIsChannelOwner)
    : session{std::move(sessionInit)},
      stateFactory{std::move(stateFactoryInit)},
      stateMachine{std::move(stateMachineInit)},
      currentChannelId{initialChannelId},
      currentChannelName(initialChannelName),
      isOwnerOfCurrentChannel{initialIsChannelOwner}
{
}

void ChannelController::activate()
{
    session->addMessageHandler({common::messages::MessageId::LeftTheChannelResponse, leftChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftChannelResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::DeleteTheChannelResponse, deleteChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftChannelResponse(msg); }});

    emit setChannel(currentChannelName.c_str(), currentChannelId.c_str(), isOwnerOfCurrentChannel);
}

void ChannelController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::LeftTheChannelResponse, leftChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::DeleteTheChannelResponse, deleteChannelResponseHandlerName});
}

const QString& ChannelController::getName() const
{
    return name;
}

void ChannelController::goToChannel(const QString& channelName, const QString& channelId, bool isOwner)
{
    LOG_S(INFO) << std::format("Set channel {} with id {} as current", channelName.toStdString(),
                               channelId.toStdString());

    isOwnerOfCurrentChannel = isOwner;
    currentChannelId = channelId.toStdString();
    currentChannelName = channelName.toStdString();

    emit setChannel(channelName, channelId, isOwner);
}

void ChannelController::goToPrivateMessages()
{
    LOG_S(INFO) << "Go to private messages";

    stateMachine->returnToThePreviousState();
}

void ChannelController::addToChannel()
{
    LOG_S(INFO) << "Add to chat";

    stateMachine->addNextState(stateFactory.createInviteToChannelState(currentChannelId));
}

void ChannelController::leaveChannel()
{

    LOG_S(INFO) << "Left the channel";

    nlohmann::json data{
        {"channelId", currentChannelId},
    };

    session->sendMessage(common::messages::MessageId::LeftTheChannel, data);
}

void ChannelController::deleteChannel()
{

    LOG_S(INFO) << "Delete the channel";

    nlohmann::json data{
        {"channelId", currentChannelId},
    };

    session->sendMessage(common::messages::MessageId::DeleteTheChannel, data);
}

void ChannelController::handleLeftChannelResponse(const common::messages::Message& /*message*/)
{
    stateMachine->returnToThePreviousState();
}
}