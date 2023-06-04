#pragma once

#include <boost/asio.hpp>
#include <functional>

#include "Message.h"

namespace common::messages
{
class MessageReader
{
public:
    virtual ~MessageReader() = default;

    virtual void startReadingMessages(std::function<void(const common::messages::Message&)> onReadMessage) = 0;

    virtual boost::asio::ip::tcp::socket& getSocket() = 0;
};
}