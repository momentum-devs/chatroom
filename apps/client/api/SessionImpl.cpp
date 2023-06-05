#include "SessionImpl.h"

#include "loguru.hpp"
#include "messages/Message.h"
#include "RemoveHandlerError.h"

namespace client::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<SocketConnector> socketConnectorInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      socketConnector{std::move(socketConnectorInit)}
{
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

void SessionImpl::handleMessage(const common::messages::Message& message)
{
    LOG_S(INFO) << "Received message: " << message;

    if (messageHandlers.contains(message.id))
    {
        for (auto [_, handler] : messageHandlers.at(message.id))
        {
            handler(message);
        }
    }
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

    messageHandlers.at(messageHandlerPayload.messageId).erase(messageHandlerPayload.name);

    if (messageHandlers.at(messageHandlerPayload.messageId).empty())
    {
        messageHandlers.erase(messageHandlerPayload.messageId);
    }
}
}