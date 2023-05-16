#pragma once

#include "MessageSerializer.h"

namespace common::messages
{
class MessageSerializerImpl final : public MessageSerializer
{
public:
    bytes::Bytes serialize(const Message& message) const override;
    Message deserialize(const bytes::Bytes& message) const override;
};
}