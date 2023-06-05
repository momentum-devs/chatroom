#pragma once

#include <boost/asio.hpp>
#include <string>

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
    virtual void addMessageHandler(const MessageHandlerPayload& messageHandlerPayload) = 0;
    virtual void removeMessageHandler(const MessageHandlerPayload& messageHandlerPayload) = 0;
};
}