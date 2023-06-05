#pragma once

#include <map>

#include "common/messages/MessageReader.h"
#include "common/messages/MessageSender.h"
#include "Session.h"
#include "SocketConnector.h"

namespace client::api
{
class SessionImpl : public Session
{
public:
    SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReader,
                std::unique_ptr<common::messages::MessageSender> messageSender,
                std::unique_ptr<SocketConnector> socketConnector);
    void connect(const ConnectorPayload& connectorPayload) override;
    void sendMessage(const common::messages::Message& message) override;
    void addMessageHandler(const MessageHandlerPayload& messageHandlerPayload) override;
    void removeMessageHandler(const MessageHandlerPayload& messageHandlerPayload) override;

private:
    void handleMessage(const common::messages::Message& message);

    std::unique_ptr<common::messages::MessageReader> messageReader;
    std::unique_ptr<common::messages::MessageSender> messageSender;
    std::unique_ptr<SocketConnector> socketConnector;
    std::map<common::messages::MessageId, std::map<std::string, std::function<void(const common::messages::Message&)>>>
        messageHandlers;
};
}
