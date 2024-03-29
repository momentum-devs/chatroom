#pragma once

#include <map>

#include "common//bytes/Bytes.h"
#include "common/messages/MessageReader.h"
#include "common/messages/MessageSender.h"
#include "MessageFactory.h"
#include "Session.h"
#include "SocketConnector.h"

namespace client::api
{
class SessionImpl : public Session
{
public:
    SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReader,
                std::unique_ptr<common::messages::MessageSender> messageSender,
                std::unique_ptr<SocketConnector> socketConnector, std::unique_ptr<MessageFactory> messageFactory);
    ~SessionImpl();
    void connect(const ConnectorPayload& connectorPayload) override;
    void sendMessage(const common::messages::Message& message) override;
    void sendMessage(common::messages::MessageId messageId, const nlohmann::json& data) override;
    void addMessageHandler(const MessageHandlerPayload& messageHandlerPayload) override;
    void removeMessageHandler(const MessageHandlerPayload& messageHandlerPayload) override;
    void storeToken(const std::string& token) override;
    void storeUser(const types::User& user) override;
    QObject* getUser() override;
    void logout() override;

private:
    void handleMessage(const common::messages::Message& message);

    std::unique_ptr<common::messages::MessageReader> messageReader;
    std::unique_ptr<common::messages::MessageSender> messageSender;
    std::unique_ptr<SocketConnector> socketConnector;
    std::unique_ptr<MessageFactory> messageFactory;
    std::map<common::messages::MessageId, std::map<std::string, std::function<void(const common::messages::Message&)>>>
        messageHandlers;
    std::optional<std::string> token;
    std::mutex lock;
    std::vector<MessageHandlerPayload> messageHandlersToDelete;
    types::User user;
};
}
