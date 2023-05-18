#pragma once

#include "Message.h"

namespace common::messages
{
class MessageSerializer
{
public:
    virtual ~MessageSerializer() = default;
    virtual bytes::Bytes serialize(const Message& message) const = 0;
    virtual Message deserialize(const bytes::Bytes& message) const = 0 ;
};
}