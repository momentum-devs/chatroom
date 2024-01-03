#include "ChannelController.h"

#include "fmt/format.h"
#include "loguru.hpp"

namespace client::gui
{
ChannelController::ChannelController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                     std::shared_ptr<StateMachine> stateMachineInit,
                                     std::shared_ptr<storage::ConversationStorage> conversationStorageInit,
                                     const std::string& initialChannelId, const std::string& initialChannelName,
                                     bool initialIsChannelOwner)
    : session{std::move(sessionInit)},
      stateFactory{std::move(stateFactoryInit)},
      stateMachine{std::move(stateMachineInit)},
      conversationStorage{std::move(conversationStorageInit)},
      currentChannelId{initialChannelId},
      currentChannelName(initialChannelName),
      isOwnerOfCurrentChannel{initialIsChannelOwner},
      isActivated{false}
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

    session->addMessageHandler({common::messages::MessageId::SendChannelMessageResponse,
                                sendChannelMessageResponseHandlerName,
                                [this](const auto& msg) { handleSendChannelMessageResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetChannelMessagesResponse,
                                getChannelMessagesResponseHandlerName,
                                [this](const auto& msg) { handleGetChannelMessagesResponse(msg); }});

    goToChannel(currentChannelName.c_str(), currentChannelId.c_str(), isOwnerOfCurrentChannel);

    isActivated = true;
}

void ChannelController::deactivate()
{
    session->removeMessageHandler(
        {common::messages::MessageId::LeftTheChannelResponse, leftChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::DeleteTheChannelResponse, deleteChannelResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetChannelMembersResponse, getChannelMembersResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::SendChannelMessageResponse, sendChannelMessageResponseHandlerName});

    session->removeMessageHandler(
        {common::messages::MessageId::GetChannelMessagesResponse, getChannelMessagesResponseHandlerName});
}

const QString& ChannelController::getName() const
{
    return name;
}

void ChannelController::goToChannel(const QString& channelName, const QString& channelId, bool isOwner)
{
    LOG_S(INFO) << fmt::format("Set channel {} with id {} as current", channelName.toStdString(),
                               channelId.toStdString());

    if (not isActivated or currentChannelId != channelId.toStdString())
    {
        if (conversationStorage->hasConversation(currentChannelId))
        {
            messageStorage = conversationStorage->getConversation(currentChannelId);
        }
        else
        {
            messageStorage = conversationStorage->createConversation(currentChannelId);
        }

        emit setMessageStorage(messageStorage);
    }

    currentChannelId = channelId.toStdString();
    isOwnerOfCurrentChannel = isOwner;
    currentChannelName = channelName.toStdString();

    emit setChannel(channelName, channelId, isOwner);

    session->sendMessage(common::messages::MessageId::GetChannelMembers,
                         nlohmann::json{{"channelId", currentChannelId}});

    session->sendMessage(common::messages::MessageId::GetChannelMessages,
                         nlohmann::json{{"channelId", currentChannelId}, {"offset", 0}, {"limit", 50}});
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
                LOG_S(INFO) << fmt::format("Adding member {} with id {} to list",
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

void ChannelController::sendChannelMessage(const QString& messageText)
{
    LOG_S(INFO) << "Send channel message";

    nlohmann::json data{
        {"channelId", currentChannelId},
        {"text", messageText.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::SendChannelMessage, data);
}

void ChannelController::handleSendChannelMessageResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received send channel message response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << "Error while sending channel message: " << responseJson.at("error").get<std::string>();
    }
    else if (responseJson.contains("data") and responseJson.at("data").contains("message"))
    {
        LOG_S(INFO) << "Successfully sent channel message";

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

void ChannelController::handleGetChannelMessagesResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received channel messages list";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << "Error while getting channel messages: " << responseJson.at("error").get<std::string>();
    }

    if (responseJson.contains("data") and responseJson.at("data").contains("messages"))
    {
        if (not responseJson.at("data").at("messages").empty() and
            messageStorage->hasMessage(responseJson.at("data").at("messages").back().at("id").get<std::string>()) and
            messageStorage->hasMessage(responseJson.at("data").at("messages").front().at("id").get<std::string>()))
        {
            LOG_S(INFO) << "Messages already loaded";

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
}
