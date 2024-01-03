#include "LeftColumnController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
LeftColumnController::LeftColumnController(std::shared_ptr<api::Session> sessionInit,
                                           const StateFactory& stateFactoryInit,
                                           std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void LeftColumnController::activate()
{
    session->addMessageHandler({common::messages::MessageId::GetUserChannelsResponse,
                                getUserChannelsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetUserChannelInvitationsResponse,
                                getUserChannelInvitationsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelInvitationsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::ChangeChannelInvitationResponse,
                                changeChannelInvitationResponseHandlerName,
                                [this](const auto& msg) { handleChangeChannelInvitationResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});
    session->sendMessage(common::messages::MessageId::GetUserChannelInvitations, {});

    auto userName = dynamic_cast<types::User*>(session->getUser())->property("nickname").toString();

    emit setUserName(userName);
}

void LeftColumnController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::GetUserChannelsResponse, getUserChannelsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetUserChannelInvitationsResponse, getUserChannelInvitationsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::ChangeChannelInvitationResponse, changeChannelInvitationResponseHandlerName});
}

const QString& LeftColumnController::getName() const
{
    return name;
}

void LeftColumnController::goToPrivateMessages()
{
    emit goToPrivateMessagesSignal();
}

void LeftColumnController::goToCreateChannel()
{
    LOG_S(INFO) << "Handle go to create channel";

    stateMachine->addNextState(stateFactory.createCreateChannelState());
}

void LeftColumnController::goToUserSettings()
{
    LOG_S(INFO) << "Handle go to user settings";

    stateMachine->addNextState(stateFactory.createUserSettingsState());
}

void LeftColumnController::goToChannel(const QString& channelName, const QString& channelId, bool isOwner)
{
    emit goToChannelSignal(channelName, channelId, isOwner);
}

void LeftColumnController::handleGetUserChannelsResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle get user's channel data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user data: {}", responseJson.at("error").get<std::string>());
    }

    emit clearChannelList();

    if (responseJson.contains("data"))
    {
        for (const auto& channel : responseJson.at("data"))
        {
            if (channel.contains("id") and channel.contains("name") and channel.contains("isOwner"))
            {
                LOG_S(INFO) << fmt::format("Adding channel {} with id {} to list",
                                           channel.at("name").get<std::string>(), channel.at("id").get<std::string>());

                emit addChannel(QString::fromStdString(channel.at("name").get<std::string>()),
                                QString::fromStdString(channel.at("id").get<std::string>()),
                                channel.at("isOwner").get<bool>());
            }
            else
            {
                LOG_S(ERROR) << "Wrong channel format";
            }
        }
    }
    else
    {
        LOG_S(ERROR) << "Response without data";
    }
}

void LeftColumnController::handleGetUserChannelInvitationsResponse(const common::messages::Message& message)
{
    emit clearChannelInvitationList();

    LOG_S(INFO) << "Handle get user's channel invitation data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user channel's invitations: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& channel : responseJson.at("data"))
        {
            if (channel.contains("id") and channel.contains("name"))
            {
                LOG_S(INFO) << fmt::format("Adding channel invitation {} with id {} to list",
                                           channel.at("name").get<std::string>(), channel.at("id").get<std::string>());

                emit addChannelInvitation(QString::fromStdString(channel.at("name").get<std::string>()),
                                          QString::fromStdString(channel.at("id").get<std::string>()));
            }
            else
            {
                LOG_S(ERROR) << "Wrong channel invitation format";
            }
        }
    }
    else
    {
        LOG_S(ERROR) << "Response without data";
    }
}

void LeftColumnController::handleChangeChannelInvitationResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle change channel invitation response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user channel's invitations: {}",
                                    responseJson.at("error").get<std::string>());
    }
    else
    {
        session->sendMessage(common::messages::MessageId::GetUserChannels, {});
        session->sendMessage(common::messages::MessageId::GetUserChannelInvitations, {});
    }
}

void LeftColumnController::acceptChannelInvitation(const QString& channelId)
{
    LOG_S(INFO) << fmt::format("Accept invitation to channel id {}", channelId.toStdString());

    nlohmann::json data{
        {"channelId", channelId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptChannelInvitation, data);
}

void LeftColumnController::rejectChannelInvitation(const QString& channelId)
{
    LOG_S(INFO) << fmt::format("Reject invitation to channel id {}", channelId.toStdString());

    nlohmann::json data{
        {"channelId", channelId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectChannelInvitation, data);
}
}
