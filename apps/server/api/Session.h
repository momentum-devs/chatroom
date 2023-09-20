#pragma once

#include <boost/asio.hpp>

#include "common/messages/Message.h"

namespace server::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void startSession() = 0;
    virtual void sendMessage(const common::messages::Message& message) = 0;
    virtual bool isActive() = 0;
    virtual void close() = 0;
};
}
