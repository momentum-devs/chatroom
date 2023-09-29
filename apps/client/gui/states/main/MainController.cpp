#include "MainController.h"

#include <chrono>
#include <nlohmann/json.hpp>
#include <thread>

#include "loguru.hpp"

namespace client::gui
{
MainController::MainController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
    nextState = stateFactory.createVerifyUserState();
}

void MainController::activate()
{
    session->addMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataResponseHandlerName,
                                [this](const auto& msg) { handleGetUserDataResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserData, {});

    session->addMessageHandler({common::messages::MessageId::GetUserChannelsResponse,
                                getUserChannelsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelsResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});

    session->addMessageHandler({common::messages::MessageId::LeftTheChannelResponse, leftTheChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftTheChannelResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::DeleteTheChannelResponse,
                                deleteTheChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftTheChannelResponse(msg); }});
}

void MainController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::GetUserChannelsResponse, getUserChannelsResponseHandlerName});

    session->removeMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::LeftTheChannelResponse, leftTheChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::DeleteTheChannelResponse, deleteTheChannelResponseHandlerName});
}

void MainController::logout()
{
    LOG_S(INFO) << "Handle logout";

    session->logout();

    stateMachine->returnToThePreviousState();
}

void MainController::goToCreateChannel()
{
    LOG_S(INFO) << "Handle go to create channel";

    stateMachine->addNextState(stateFactory.createCreateChannelState());
}

void MainController::handleGetUserChannelsResponse(const common::messages::Message& message)
{
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(10ms);

    LOG_S(INFO) << "Handle get user's channel data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user data: {}", responseJson.at("error").get<std::string>());
    }

    emit clearChannelList();

    if (responseJson.contains("data"))
    {
        for (const auto& channel : responseJson.at("data"))
        {
            if (channel.contains("id") and channel.contains("name") and channel.contains("isOwner"))
            {
                LOG_S(INFO) << std::format("Adding channel {} with id {} to list",
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

void MainController::handleGetUserDataResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle get user data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user data: {}", responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data") and responseJson.at("data").contains("verified") and
        not responseJson.at("data").at("verified").get<bool>())
    {
        LOG_S(INFO) << "User is not verified, go to verification";

        stateMachine->addNextState(nextState.value());
    }
    else
    {
        LOG_S(INFO) << "User is verified";
    }
}

void MainController::goToSendFriendRequest()
{
    LOG_S(INFO) << "Handle go to send friend request";

    stateMachine->addNextState(stateFactory.createSendFriendRequestState());
}

void MainController::goToUserSettings()
{
    LOG_S(INFO) << "Handle go to user settings";

    stateMachine->addNextState(stateFactory.createUserSettingsState());
}

void MainController::setCurrentChannel(const QString& channelId)
{
    LOG_S(INFO) << std::format("Set current chat to id {}", channelId.toStdString());

    currentChannelId = channelId.toStdString();
}

void MainController::addToChannel()
{
    LOG_S(INFO) << "Add to chat";

    stateMachine->addNextState(stateFactory.createInviteToChannelState(currentChannelId));
}

void MainController::leftTheChannel()
{
    LOG_S(INFO) << "Left the channel";

    nlohmann::json data{
        {"channelId", currentChannelId},
    };

    session->sendMessage(common::messages::MessageId::LeftTheChannel, data);
}

void MainController::deleteTheChannel()
{
    LOG_S(INFO) << "Delete the channel";

    nlohmann::json data{
        {"channelId", currentChannelId},
    };

    session->sendMessage(common::messages::MessageId::DeleteTheChannel, data);
}

void MainController::handleLeftTheChannelResponse(const common::messages::Message& message)
{
    currentChannelId = "";

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});
}
}