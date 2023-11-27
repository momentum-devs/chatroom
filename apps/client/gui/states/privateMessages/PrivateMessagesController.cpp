#include "PrivateMessagesController.h"

#include "loguru.hpp"

namespace client::gui
{
PrivateMessagesController::PrivateMessagesController(std::shared_ptr<api::Session> sessionInit,
                                                     const StateFactory& stateFactoryInit,
                                                     std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void PrivateMessagesController::activate()
{
    session->addMessageHandler({common::messages::MessageId::GetUserFriendsResponse, getUserFriendsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserFriendsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetFriendInvitationsResponse,
                                getUserFriendInvitationsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserFriendInvitationsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::ChangeFriendInvitationsResponse,
                                changeFriendInvitationResponseHandlerName,
                                [this](const auto& msg) { handleChangeFriendInvitationResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::RemoveFromFriendsResponse,
                                removeFromFriendsResponseHandlerName,
                                [this](const auto&) { handleRemoveFromFriendsResponse(); }});

    session->sendMessage(common::messages::MessageId::GetUserFriends, {});
    session->sendMessage(common::messages::MessageId::GetFriendInvitations, {});

    if (not currentFriendId.empty())
    {
        emit setCurrentFriendName(currentFriendName.c_str());
    }
}

void PrivateMessagesController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::GetUserFriendsResponse, getUserFriendsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetFriendInvitationsResponse, getUserFriendInvitationsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::ChangeFriendInvitationsResponse, changeFriendInvitationResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::RemoveFromFriendsResponse, removeFromFriendsResponseHandlerName});
}

void PrivateMessagesController::goToChannel(const QString& channelName, const QString& channelId, bool isOwner)
{
    LOG_S(INFO) << std::format("PrivateMessagesController: Go to channel {} with id {}", channelName.toStdString(),
                               channelId.toStdString());

    stateMachine->addNextState(
        stateFactory.createChannelState(channelId.toStdString(), channelName.toStdString(), isOwner));
}

const QString& PrivateMessagesController::getName() const
{
    return name;
}

void PrivateMessagesController::goToSendFriendInvitation()
{
    LOG_S(INFO) << "Handle go to send friend request";

    stateMachine->addNextState(stateFactory.createSendFriendInvitationState());
}

void PrivateMessagesController::acceptFriendInvitation(const QString& requestId)
{
    LOG_S(INFO) << std::format("Accept friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptFriendInvitations, data);
}

void PrivateMessagesController::rejectFriendInvitation(const QString& requestId)
{
    LOG_S(INFO) << std::format("Reject friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectFriendInvitations, data);
}

void PrivateMessagesController::setCurrentFriend(const QString& friendId, const QString& friendName)
{
    LOG_S(INFO) << std::format("Set current friend to {} with id {}", friendName.toStdString(), friendId.toStdString());

    currentFriendId = friendId.toStdString();
    currentFriendName = friendName.toStdString();

    emit setCurrentFriendName(friendName);
}

void PrivateMessagesController::removeFromFriends()
{
    LOG_S(INFO) << std::format("Remove user with id {} from friends", currentFriendId);

    nlohmann::json data{
        {"userFriendId", currentFriendId},
    };

    session->sendMessage(common::messages::MessageId::RemoveFromFriends, data);
}

void PrivateMessagesController::handleGetUserFriendsResponse(const common::messages::Message& message)
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

void PrivateMessagesController::handleGetUserFriendInvitationsResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received friend requests list";

    emit clearFriendInvitationList();

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << std::format("Error while getting user friend requests: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& friendInvitation : responseJson.at("data"))
        {
            if (friendInvitation.contains("id") and friendInvitation.contains("name"))
            {
                LOG_S(INFO) << std::format("Adding friend request {} with id {} to list",
                                           friendInvitation.at("name").get<std::string>(),
                                           friendInvitation.at("id").get<std::string>());

                emit addFriendInvitation(QString::fromStdString(friendInvitation.at("name").get<std::string>()),
                                         QString::fromStdString(friendInvitation.at("id").get<std::string>()));
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

void PrivateMessagesController::handleChangeFriendInvitationResponse(const common::messages::Message& message)
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
        session->sendMessage(common::messages::MessageId::GetFriendInvitations, {});
    }
}

void PrivateMessagesController::handleRemoveFromFriendsResponse()
{
    LOG_S(INFO) << "Handle remove friend response";

    currentFriendId = "";
    currentFriendName = "";

    session->sendMessage(common::messages::MessageId::GetUserFriends, {});

    emit removedFromFriends();
}
}
