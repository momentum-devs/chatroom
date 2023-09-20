#include "SessionImpl.h"

#include "loguru.hpp"
#include "RemoveHandlerError.h"

namespace client::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<SocketConnector> socketConnectorInit,
                         std::unique_ptr<MessageFactory> messageFactoryInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      socketConnector{std::move(socketConnectorInit)},
      messageFactory{std::move(messageFactoryInit)}
{
}

SessionImpl::~SessionImpl()
{
    logout();
}

void SessionImpl::connect(const ConnectorPayload& connectorPayload)
{
    socketConnector->connect(connectorPayload);

    messageReader->startReadingMessages([this](const common::messages::Message& message) { handleMessage(message); });
}

void SessionImpl::sendMessage(const common::messages::Message& message)
{
    messageSender->sendMessage(message);
}

void SessionImpl::sendMessage(common::messages::MessageId messageId, const nlohmann::json& data)
{
    auto message = messageFactory->createMessage(messageId, data, token);

    sendMessage(message);
}

void SessionImpl::handleMessage(const common::messages::Message& message)
{
    std::lock_guard<std::mutex> guard(lock);

    LOG_S(INFO) << std::format("Received message: (id: {0:}, payload: {1:}), id {0:} has {2:} handlers",
                               toString(message.id), static_cast<std::string>(message.payload),
                               (messageHandlers.contains(message.id) ? messageHandlers.at(message.id).size() : 0));

    if (messageHandlers.contains(message.id))
    {
        for (auto [_, handler] : messageHandlers.at(message.id))
        {
            handler(message);
        }
    }

    for (const auto& messageHandlerToDelete : messageHandlersToDelete)
    {
        messageHandlers.at(messageHandlerToDelete.messageId).erase(messageHandlerToDelete.name);

        if (messageHandlers.at(messageHandlerToDelete.messageId).empty())
        {
            messageHandlers.erase(messageHandlerToDelete.messageId);
        }
    }

    messageHandlersToDelete.clear();
}

void SessionImpl::addMessageHandler(const MessageHandlerPayload& messageHandlerPayload)
{
    LOG_S(INFO) << std::format("Add handler for MessageID {} for name \"{}\"",
                               common::messages::toString(messageHandlerPayload.messageId), messageHandlerPayload.name);

    if (not messageHandlers.contains(messageHandlerPayload.messageId))
    {
        messageHandlers.insert({messageHandlerPayload.messageId, {}});
    }

    messageHandlers.at(messageHandlerPayload.messageId)
        .insert({messageHandlerPayload.name, messageHandlerPayload.handler});
}

void SessionImpl::removeMessageHandler(const MessageHandlerPayload& messageHandlerPayload)
{
    LOG_S(INFO) << std::format("Remove handler for MessageID {} for name \"{}\"",
                               common::messages::toString(messageHandlerPayload.messageId), messageHandlerPayload.name);

    if (not messageHandlers.contains(messageHandlerPayload.messageId) or
        not messageHandlers.at(messageHandlerPayload.messageId).contains(messageHandlerPayload.name))
    {
        throw RemoveHandlerError{
            std::format("Message handlers does not contain handler for MessageID {} for name \"{}\"",
                        common::messages::toString(messageHandlerPayload.messageId), messageHandlerPayload.name)};
    }

    std::unique_lock<std::mutex> guard(lock, std::try_to_lock);

    if (guard.owns_lock())
    {
        messageHandlers.at(messageHandlerPayload.messageId).erase(messageHandlerPayload.name);

        if (messageHandlers.at(messageHandlerPayload.messageId).empty())
        {
            messageHandlers.erase(messageHandlerPayload.messageId);
        }

        messageHandlersToDelete.clear();
    }
    else
    {
        messageHandlersToDelete.push_back(messageHandlerPayload);
    }
}

void SessionImpl::storeToken(const std::string& tokenInit)
{
    token = tokenInit;
}

void SessionImpl::logout()
{
    if (token)
    {
        sendMessage(common::messages::MessageId::Logout, {});
    }

    token = std::nullopt;
}
}