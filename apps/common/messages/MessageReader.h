#pragma once

#include <functional>

#include "Message.h"

namespace common::messages
{
class MessageReader
{
public:
    virtual ~MessageReader() = default;
    virtual void startReadingMessages(std::function<void(const common::messages::Message&)> onReadMessage)  = 0;
};
}