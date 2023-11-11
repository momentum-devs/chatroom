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

    session->addMessageHandler({common::messages::MessageId::GetFriendRequestsResponse,
                                getUserFriendRequestsResponseHandlerName,
                                [this](const auto& msg) { handleGetUserFriendRequestsResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::ChangeFriendRequestsResponse,
                                changeFriendRequestResponseHandlerName,
                                [this](const auto& msg) { handleChangeFriendRequestResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::RemoveFromFriendsResponse,
                                removeFromFriendsResponseHandlerName,
                                [this](const auto& msg) { handleRemoveFromFriendsResponse(msg); }});

    session->sendMessage(common::messages::MessageId::GetUserFriends, {});
    session->sendMessage(common::messages::MessageId::GetFriendRequests, {});
}

void PrivateMessagesController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::GetUserFriendsResponse, getUserFriendsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetFriendRequestsResponse, getUserFriendRequestsResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::ChangeFriendRequestsResponse, changeFriendRequestResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::RemoveFromFriendsResponse, removeFromFriendsResponseHandlerName});
}

void PrivateMessagesController::goToChannel(const QString& channelId)
{
    LOG_S(INFO) << std::format("PrivateMessagesController: Go to chanel with id {}", channelId.toStdString());

    // TODO: implement
}

const QString& PrivateMessagesController::getName() const
{
    return name;
}

void PrivateMessagesController::goToSendFriendRequest()
{
    LOG_S(INFO) << "Handle go to send friend request";

    stateMachine->addNextState(stateFactory.createSendFriendRequestState());
}

void PrivateMessagesController::acceptFriendRequest(const QString& requestId)
{
    LOG_S(INFO) << std::format("Accept friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptFriendRequests, data);
}

void PrivateMessagesController::rejectFriendRequest(const QString& requestId)
{
    LOG_S(INFO) << std::format("Reject friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectFriendRequests, data);
}

void PrivateMessagesController::setCurrentFriend(const QString& friendId, const QString& friendName)
{
    LOG_S(INFO) << std::format("Set current friend to {} with id {}", friendName.toStdString(), friendId.toStdString());

    currentFriendId = friendId.toStdString();

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

void PrivateMessagesController::handleGetUserFriendRequestsResponse(const common::messages::Message& message)
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

void PrivateMessagesController::handleChangeFriendRequestResponse(const common::messages::Message& message)
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

void PrivateMessagesController::handleRemoveFromFriendsResponse(const common::messages::Message& message)
{
    currentFriendId = "";

    session->sendMessage(common::messages::MessageId::GetUserFriends, {});

    emit removedFromFriends();
}
}