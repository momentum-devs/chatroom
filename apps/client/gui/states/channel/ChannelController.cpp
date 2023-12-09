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

    session->addMessageHandler({common::messages::MessageId::GetChannelMembersResponse,
                                getChannelMembersResponseHandlerName,
                                [this](const auto& msg) { handleGetChannelMembersResponse(msg); }});

    goToChannel(currentChannelName.c_str(), currentChannelId.c_str(), isOwnerOfCurrentChannel);
}

void ChannelController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::LeftTheChannelResponse, leftChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::DeleteTheChannelResponse, deleteChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetChannelMembersResponse, getChannelMembersResponseHandlerName});
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

    nlohmann::json data{
        {"channelId", currentChannelId},
    };

    session->sendMessage(common::messages::MessageId::GetChannelMembers, data);
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
    LOG_S(INFO) << "Handle leave channel message";

    stateMachine->returnToThePreviousState();
}

void ChannelController::handleGetChannelMembersResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received channel members list";

    emit clearMembersList();

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << "Error while getting channel members: " << responseJson.at("error").get<std::string>();
    }

    if (responseJson.contains("data"))
    {
        for (const auto& channelMember : responseJson.at("data"))
        {
            if (channelMember.contains("id") and channelMember.contains("name") and channelMember.contains("isActive"))
            {
                LOG_S(INFO) << std::format("Adding member {} with id {} to list",
                                           channelMember.at("name").get<std::string>(),
                                           channelMember.at("id").get<std::string>());

                emit addMember(QString::fromStdString(channelMember.at("name").get<std::string>()),
                               QString::fromStdString(channelMember.at("id").get<std::string>()),
                               channelMember.at("isActive").get<bool>());
            }
            else
            {
                LOG_S(ERROR) << "Wrong friend request format";
            }
        }
    }
    else
    {
        LOG_S(ERROR) << "Response without data";
    }
}

void ChannelController::sendChannelMessage(types::Message& message)
{
    LOG_S(INFO) << "Send channel message";

    auto text = message.property("messageText").toString().toStdString();

    nlohmann::json data{
        {"channelId", currentChannelId},
        {"text", text},
    };

    session->sendMessage(common::messages::MessageId::SendChannelMessage, data);
}
}