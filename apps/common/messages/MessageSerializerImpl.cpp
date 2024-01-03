#include "MessageSerializerImpl.h"

#include "errors/InvalidChecksumError.h"
#include "fmt/format.h"

namespace common::messages
{
bytes::Bytes MessageSerializerImpl::serialize(const Message& message) const
{
    return bytes::Bytes(static_cast<unsigned char>(message.id)) + message.payload + message.calculateCheckSum();
}

Message MessageSerializerImpl::deserialize(const bytes::Bytes& messageBytes) const
{
    size_t cumulativeSum = 0;

    auto id = static_cast<MessageId>(messageBytes[cumulativeSum]);

    cumulativeSum += Message::idSize;

    auto payloadFieldSize = messageBytes.size() - cumulativeSum - Message::checksumSize;

    auto payload = messageBytes.subBytes(cumulativeSum, payloadFieldSize);

    cumulativeSum += payloadFieldSize;

    auto message = Message{id, payload};

    auto checksum = messageBytes.subBytes(cumulativeSum, Message::checksumSize);

    if (checksum != message.calculateCheckSum())
    {
        throw InvalidChecksumError{"Invalid checksum"};
    }

    return message;
}
}
