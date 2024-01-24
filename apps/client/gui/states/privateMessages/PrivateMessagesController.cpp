#include "PrivateMessagesController.h"

#include "fmt/format.h"
#include "loguru.hpp"

namespace client::gui
{
PrivateMessagesController::PrivateMessagesController(
    std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
    std::shared_ptr<StateMachine> stateMachineInit,
    std::shared_ptr<storage::ConversationStorage> conversationStorageInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      messageStorage{std::make_shared<storage::MessageStorage>()},
      conversationStorage{std::move(conversationStorageInit)}
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

    session->addMessageHandler({common::messages::MessageId::SendPrivateMessageResponse,
                                sendPrivateMessageResponseHandlerName,
                                [this](const auto& msg) { handleSendPrivateMessageResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetPrivateMessagesResponse,
                                getPrivateMessagesResponseHandlerName,
                                [this](const auto& msg) { handleGetPrivateMessagesResponse(msg); }});

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

    session->removeMessageHandler(
        {common::messages::MessageId::SendPrivateMessageResponse, sendPrivateMessageResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetPrivateMessagesResponse, getPrivateMessagesResponseHandlerName});
}

void PrivateMessagesController::goToChannel(const QString& channelName, const QString& channelId, bool isOwner)
{
    LOG_S(INFO) << fmt::format("PrivateMessagesController: Go to channel {} with id {}", channelName.toStdString(),
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
    LOG_S(INFO) << fmt::format("Accept friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::AcceptFriendInvitations, data);
}

void PrivateMessagesController::rejectFriendInvitation(const QString& requestId)
{
    LOG_S(INFO) << fmt::format("Reject friend request with id {}", requestId.toStdString());

    nlohmann::json data{
        {"requestId", requestId.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::RejectFriendInvitations, data);
}

void PrivateMessagesController::setCurrentFriend(const QString& friendId, const QString& friendName,
                                                 const QString& groupId)
{
    LOG_S(INFO) << fmt::format("Set current friend to {} with id {}", friendName.toStdString(), friendId.toStdString());

    if (currentFriendId != friendId.toStdString())
    {
        if (not conversationStorage->hasConversation(currentFriendId))
        {
            messageStorage = conversationStorage->createConversation(currentFriendId);
        }
        else
        {
            messageStorage = conversationStorage->getConversation(currentFriendId);
        }

        emit setMessageStorage(messageStorage);
    }

    currentFriendId = friendId.toStdString();
    currentFriendName = friendName.toStdString();
    currentFriendGroupId = groupId.toStdString();

    emit setCurrentFriendName(friendName);

    session->sendMessage(common::messages::MessageId::GetPrivateMessages,
                         {{"groupId", currentFriendGroupId}, {"limit", 50}, {"offset", 0}});
}

void PrivateMessagesController::removeFromFriends()
{
    LOG_S(INFO) << fmt::format("Remove user with id {} from friends", currentFriendId);

    nlohmann::json data{
        {"userFriendId", currentFriendId},
    };

    session->sendMessage(common::messages::MessageId::RemoveFromFriends, data);
}

void PrivateMessagesController::handleGetUserFriendsResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << fmt::format("Received friend list");

    emit clearFriendList();

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user friends: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& userFriend : responseJson.at("data"))
        {
            if (userFriend.contains("id") and userFriend.contains("name") and userFriend.contains("isActive") and
                userFriend.contains("groupId"))
            {
                LOG_S(INFO) << fmt::format("Adding friend {} with id {} to list",
                                           userFriend.at("name").get<std::string>(),
                                           userFriend.at("id").get<std::string>());

                emit addFriend(QString::fromStdString(userFriend.at("name").get<std::string>()),
                               QString::fromStdString(userFriend.at("id").get<std::string>()),
                               QString::fromStdString(userFriend.at("groupId").get<std::string>()),
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
        LOG_S(ERROR) << fmt::format("Error while getting user friend requests: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data"))
    {
        for (const auto& friendInvitation : responseJson.at("data"))
        {
            if (friendInvitation.contains("id") and friendInvitation.contains("name"))
            {
                LOG_S(INFO) << fmt::format("Adding friend request {} with id {} to list",
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
        LOG_S(ERROR) << fmt::format("Error while change user friend request: {}",
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

void PrivateMessagesController::sendPrivateMessage(const QString& messageText)
{
    LOG_S(INFO) << fmt::format("Send private message: {} to {} with id {}", messageText.toStdString(),
                               currentFriendName, currentFriendId);

    nlohmann::json data{
        {"groupId", currentFriendGroupId},
        {"text", messageText.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::SendPrivateMessage, data);
}

void PrivateMessagesController::handleSendPrivateMessageResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle send private message response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while sending private message: {}",
                                    responseJson.at("error").get<std::string>());
    }
    else if (responseJson.contains("data") and responseJson.at("data").contains("message"))
    {
        LOG_S(INFO) << "Successfully sent private message";

        auto message = responseJson.at("data").at("message");

        if (message.contains("id") and message.contains("text") and message.contains("senderName") and
            message.contains("sentAt"))
        {
            auto dateText = QString::fromStdString(message.at("sentAt").get<std::string>());

            auto date = QDateTime::fromString(dateText, "yyyyMMddThhmmss");

            LOG_S(INFO) << fmt::format("Adding message {} with id {} sent at {} to list",
                                       message.at("text").get<std::string>(), message.at("id").get<std::string>(),
                                       message.at("sentAt").get<std::string>());

            messageStorage->addMessage(
                std::make_shared<types::Message>(QString::fromStdString(message.at("text").get<std::string>()),
                                                 QString::fromStdString(message.at("senderName").get<std::string>()),
                                                 QString::fromStdString(message.at("id").get<std::string>()), date));

            emit messagesUpdated(true);
        }
        else
        {
            LOG_S(ERROR) << "Wrong message format";
        }
    }
    else
    {
        LOG_S(ERROR) << "Response without message";
    }
}

void PrivateMessagesController::handleGetPrivateMessagesResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received private messages list";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << "Error while getting private messages: " << responseJson.at("error").get<std::string>();
    }

    if (responseJson.contains("data") and responseJson.at("data").contains("messages"))
    {
        if (not responseJson.at("data").at("messages").empty() and
            messageStorage->hasMessage(responseJson.at("data").at("messages").back().at("id").get<std::string>()) and
            messageStorage->hasMessage(responseJson.at("data").at("messages").front().at("id").get<std::string>()))
        {
            return;
        }

        for (const auto& message : responseJson.at("data").at("messages"))
        {
            if (message.contains("id") and message.contains("text") and message.contains("senderName") and
                message.contains("sentAt"))
            {
                auto dateText = QString::fromStdString(message.at("sentAt").get<std::string>());

                auto date = QDateTime::fromString(dateText, "yyyyMMddThhmmss");

                LOG_S(INFO) << fmt::format("Adding message {} with id {} sent at {} to list",
                                           message.at("text").get<std::string>(), message.at("id").get<std::string>(),
                                           message.at("sentAt").get<std::string>());

                messageStorage->addMessage(std::make_shared<types::Message>(
                    QString::fromStdString(message.at("text").get<std::string>()),
                    QString::fromStdString(message.at("senderName").get<std::string>()),
                    QString::fromStdString(message.at("id").get<std::string>()), date));
            }
            else
            {
                LOG_S(ERROR) << "Wrong message format";
            }
        }

        if (responseJson.at("data").at("messages").empty())
        {
            emit messagesUpdated(true);

            return;
        }

        auto lastMessageId = responseJson.at("data").at("messages").front().at("id").get<std::string>();

        bool shouldScrollDown = lastMessageId == messageStorage->getLatestMessage()->messageId.toStdString();

        emit messagesUpdated(shouldScrollDown);
    }
    else
    {
        LOG_S(ERROR) << "Response without messages";
    }
}
void PrivateMessagesController::getMoreMessages()
{
    auto offset = messageStorage->getMessages().size();

    session->sendMessage(common::messages::MessageId::GetPrivateMessages,
                         nlohmann::json{{"groupId", currentFriendGroupId}, {"offset", offset}, {"limit", 50}});
}
}
