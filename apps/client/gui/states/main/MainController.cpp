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

    session->addMessageHandler({common::messages::MessageId::GetUserChannelsResponse,
                                getUserChannelsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::LeftTheChannelResponse, leftTheChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftTheChannelResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetUserChannelInvitationsResponse,
                                getUserChannelInvitationsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserChannelInvitationsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::DeleteTheChannelResponse,
                                deleteTheChannelResponseHandlerName,
                                [this](const auto& msg) { handleLeftTheChannelResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::ChangeChannelInvitationResponse,
                                changeChannelInvitationResponseHandlerName,
                                [this](const auto& msg) { handleChangeChannelInvitationResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetUserFriendsResponse, getUserFriendsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserFriendsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetFriendRequestsResponse,
                                getUserFriendRequestsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserFriendRequestsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::ChangeFriendRequestsResponse,
                                changeFriendRequestResponseHandlerName,
                                [this](const auto& msg) { handleChangeFriendRequestResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::RemoveFromFriendsResponse,
                                removeFromFriendsResponseHandlerName,
                                [this](const auto& msg) { handleRemoveFromFriendsResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserData, {});
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

    session->removeMessageHandler(
        {common::messages::MessageId::GetUserChannelInvitationsResponse, getUserChannelInvitationsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::ChangeChannelInvitationResponse, changeChannelInvitationResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetUserFriendsResponse, getUserFriendsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetFriendRequestsResponse, getUserFriendRequestsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::ChangeFriendRequestsResponse, changeFriendRequestResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::RemoveFromFriendsResponse, removeFromFriendsResponseHandlerName});
}

void MainController::goToCreateChannel()
{
    LOG_S(INFO) << "Handle go to create channel";

    stateMachine->addNextState(stateFactory.createCreateChannelState());
}

void MainController::handleGetUserChannelsResponse(const common::messages::Message& message)
{
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
        LOG_S(ERROR) << std::format("Error while getting user channels: {}",
                                    responseJson.at("error").get<std::string>());
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

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});
    session->sendMessage(common::messages::MessageId::GetUserChannelInvitations, {});
    session->sendMessage(common::messages::MessageId::GetUserFriends, {});
    session->sendMessage(common::messages::MessageId::GetFriendRequests, {});
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
    LOG_S(INFO) << std::format("Set current channel to id {}", channelId.toStdString());

    currentChannelId = channelId.toStdString();

    currentFriendId = "";
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

void MainController::handleLeftTheChannelResponse(const common::messages::Message& /*message*/)
{
    currentChannelId = "";

    session->sendMessage(common::messages::MessageId::GetUserChannels, {});

    emit returnToDefaultView();
}

void MainController::handleGetUserChannelInvitationsResponse(const common::messages::Message& message)
{
    emit clearChannelInvitationList();

    LOG_S(INFO) << "Handle get user's channel invitation data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user channel's invitations: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& channel : responseJson.at("data"))
        {
            if (channel.contains("id") and channel.contains("name"))
            {
                LOG_S(INFO) << std::format("Adding channel invitation {} with id {} to list",
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

void MainController::acceptChannelInvitation(const QString& channelId)
{
    LOG_S(INFO) << std::format("Accept invitation to channel id {}", channelId.toStdString());

    nlohmann::json data{
        {"channelId", channelId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptChannelInvitation, data);
}

void MainController::rejectChannelInvitation(const QString& channelId)
{
    LOG_S(INFO) << std::format("Reject invitation to channel id {}", channelId.toStdString());

    nlohmann::json data{
        {"channelId", channelId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectChannelInvitation, data);
}

void MainController::handleChangeChannelInvitationResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle change channel invitation response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user channel's invitations: {}",
                                    responseJson.at("error").get<std::string>());
    }
    else
    {
        session->sendMessage(common::messages::MessageId::GetUserChannels, {});
        session->sendMessage(common::messages::MessageId::GetUserChannelInvitations, {});
    }
}

void MainController::handleGetUserFriendsResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << std::format("Received friend list");

    emit clearFriendList();

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user friends: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& userFriend : responseJson.at("data"))
        {
            if (userFriend.contains("id") and userFriend.contains("name") and userFriend.contains("isActive"))
            {
                LOG_S(INFO) << std::format("Adding friend {} with id {} to list",
                                           userFriend.at("name").get<std::string>(),
                                           userFriend.at("id").get<std::string>());

                emit addFriend(QString::fromStdString(userFriend.at("name").get<std::string>()),
                               QString::fromStdString(userFriend.at("id").get<std::string>()),
                               userFriend.at("isActive").get<bool>());
            }
            else
            {
                LOG_S(ERROR) << "Wrong friend data format";
            }
        }
    }
    else
    {
        LOG_S(ERROR) << "Response without data";
    }
}

void MainController::handleGetUserFriendRequestsResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << std::format("Received friend requests list");

    emit clearFriendRequestList();

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user friend requests: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& friendRequest : responseJson.at("data"))
        {
            if (friendRequest.contains("id") and friendRequest.contains("name"))
            {
                LOG_S(INFO) << std::format("Adding friend request {} with id {} to list",
                                           friendRequest.at("name").get<std::string>(),
                                           friendRequest.at("id").get<std::string>());

                emit addFriendRequest(QString::fromStdString(friendRequest.at("name").get<std::string>()),
                                      QString::fromStdString(friendRequest.at("id").get<std::string>()));
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

void MainController::acceptFriendRequest(const QString& requestId)
{
    LOG_S(INFO) << std::format("Accept friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptFriendRequests, data);
}

void MainController::rejectFriendRequest(const QString& requestId)
{
    LOG_S(INFO) << std::format("Reject friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectFriendRequests, data);
}

void MainController::handleChangeFriendRequestResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle change friend request response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while change user friend request: {}",
                                    responseJson.at("error").get<std::string>());
    }
    else
    {
        session->sendMessage(common::messages::MessageId::GetUserFriends, {});
        session->sendMessage(common::messages::MessageId::GetFriendRequests, {});
    }
}

void MainController::setCurrentFriend(const QString& friendId)
{
    LOG_S(INFO) << std::format("Set current friend to id {}", friendId.toStdString());

    currentChannelId = "";

    currentFriendId = friendId.toStdString();
}

void MainController::removeFromFriends()
{
    LOG_S(INFO) << std::format("Remove user with id {} from friends", currentFriendId);

    nlohmann::json data{
        {"userFriendId", currentFriendId},
    };

    session->sendMessage(common::messages::MessageId::RemoveFromFriends, data);
}

void MainController::handleRemoveFromFriendsResponse(const common::messages::Message& /*message*/)
{
    currentFriendId = "";

    session->sendMessage(common::messages::MessageId::GetUserFriends, {});

    emit returnToDefaultView();
}

void MainController::goToChannelMembersList()
{
    LOG_S(INFO) << std::format("Go to list of members of channel with id {}", currentChannelId);

    stateMachine->addNextState(stateFactory.createChannelMembersListState(currentChannelId));
}
}