#include "MessageSerializerImpl.h"

namespace common::messages
{
bytes::Bytes MessageSerializerImpl::serialize(const Message& message) const
{
    u_int32_t length = Message::idSize + Message::tokenSize + message.payload.size() + Message::checksumSize;

    return bytes::Bytes(length)
           + static_cast<unsigned char>(message.id)
           + message.token
           + message.payload
           + message.calculateCheckSum();
}

Message MessageSerializerImpl::deserialize(const bytes::Bytes& messageBytes) const
{
    size_t cumulativeSum = 0;


    auto length = static_cast<u_int32_t>(messageBytes.subBytes(0, sizeof(u_int32_t)));

    if(length + sizeof (u_int32_t) != messageBytes.size())
    {
        throw ; //TODO: implement
    }

    cumulativeSum += sizeof(u_int32_t);


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
        throw ; //TODO: implement
    }


    return message;
}
}
