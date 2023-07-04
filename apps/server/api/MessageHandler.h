#pragma once

#include "common/messages/Message.h"

namespace server::api
{
class MessageHandler
{
public:
    virtual ~MessageHandler() = default;

    virtual common::messages::Message handleMessage(const common::messages::Message& message) const = 0;
};
}