#pragma once

#include <boost/asio.hpp>
#include <string>

#include "common/messages/Message.h"

namespace client::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void connect(const std::string& hostName, unsigned short portNumber) = 0;

    virtual void sendMessage(const common::messages::Message& message) = 0;
};
}