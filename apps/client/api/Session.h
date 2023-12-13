#pragma once

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "client/types/User.h"
#include "ConnectorPayload.h"
#include "MessageHandlerPayload.h"

namespace client::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void connect(const ConnectorPayload& connectorPayload) = 0;
    virtual void sendMessage(const common::messages::Message& message) = 0;
    virtual void sendMessage(common::messages::MessageId messageId, const nlohmann::json& data) = 0;
    virtual void addMessageHandler(const MessageHandlerPayload& messageHandlerPayload) = 0;
    virtual void removeMessageHandler(const MessageHandlerPayload& messageHandlerPayload) = 0;
    virtual void storeToken(const std::string& token) = 0;
    virtual void storeUser(const types::User& user) = 0;
    virtual QObject* getUser() = 0;
    virtual void logout() = 0;
};
}