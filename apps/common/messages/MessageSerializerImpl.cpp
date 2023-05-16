#include "MessageSerializerImpl.h"

namespace common::messages
{
bytes::Bytes MessageSerializerImpl::serialize(const Message& message) const
{
    auto length = Message::idSize + Message::tokenSize + message.payload.size() + Message::checksumSize;

    return bytes::Bytes(length)
           + static_cast<unsigned char>(message.id)
           + message.token
           + message.payload
           + message.calculateCheckSum();
}

Message MessageSerializerImpl::deserialize(const bytes::Bytes& message) const
{
    size_t cumulativeSum = 0;

    auto length = message.subBytes(0, sizeof(size_t));
    cumulativeSum += sizeof(size_t);

    auto id = static_cast<MessageId>(message[cumulativeSum]);
    cumulativeSum += Message::idSize;

    auto token = message.subBytes(cumulativeSum, Message::tokenSize);
    cumulativeSum += Message::tokenSize;

    auto payloadFieldSize = message.size() - cumulativeSum - Message::checksumSize;
    auto payload = message.subBytes(cumulativeSum, payloadFieldSize);
    cumulativeSum += payloadFieldSize;

    auto checksum = message.subBytes(cumulativeSum, Message::checksumSize);


    return Message{id, token, payload};
}
}
