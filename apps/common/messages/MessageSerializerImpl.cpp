#include "MessageSerializerImpl.h"

#include <vector>

#include <format>

#include "errors/InvalidChecksumError.h"

namespace common::messages
{
bytes::Bytes MessageSerializerImpl::serialize(const Message& message) const
{

    return bytes::Bytes(static_cast<unsigned char>(message.id))
           + message.token
           + message.payload
           + message.calculateCheckSum();
}

Message MessageSerializerImpl::deserialize(const bytes::Bytes& messageBytes) const
{
    size_t cumulativeSum = 0;

    auto id = static_cast<MessageId>(messageBytes[cumulativeSum]);

    cumulativeSum += Message::idSize;


    auto token = messageBytes.subBytes(cumulativeSum, Message::tokenSize);

    cumulativeSum += Message::tokenSize;


    auto payloadFieldSize = messageBytes.size() - cumulativeSum - Message::checksumSize;

    auto payload = messageBytes.subBytes(cumulativeSum, payloadFieldSize);

    cumulativeSum += payloadFieldSize;

    auto message = Message{id, token, payload};

    auto checksum = messageBytes.subBytes(cumulativeSum, Message::checksumSize);

    if(checksum != message.calculateCheckSum())
    {
//        throw InvalidChecksumError{std::format("Invalid checksum: calculated={::04x}, from bytes {::04x}",
//                                               static_cast<std::vector<char>>(message.calculateCheckSum()), static_cast<std::vector<char>>(checksum))};
    }


    return message;
}
}
