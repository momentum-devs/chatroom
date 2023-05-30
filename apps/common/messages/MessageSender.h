#pragma once

#include "Message.h"

namespace common::messages
{
class MessageSender
{
public:
    virtual ~MessageSender() = default;

    virtual void sendMessage(const Message& message) = 0;
};
}