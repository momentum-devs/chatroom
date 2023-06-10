#pragma once

#include <boost/asio.hpp>

#include "messages/Message.h"

namespace server::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void startSession() = 0;
    virtual void sendMessage(const common::messages::Message& message) = 0;
};
}
