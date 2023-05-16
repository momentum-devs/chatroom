#pragma once

#include "MessageId.h"

#include "common/bytes/Bytes.h"

#include <array>

namespace common::messages
{
struct Message{
    MessageId id;
    bytes::Bytes token;
    bytes::Bytes payload;

    inline bytes::Bytes calculateCheckSum() const
    {
        //TODO: implement
        return bytes::Bytes{};
    }

    inline size_t size() const
    {
        return sizeof id + tokenSize + payload.size() + checksumSize;
    }

    inline static constexpr size_t idSize = 1;
    inline static constexpr size_t checksumSize = 40;
    inline static constexpr size_t tokenSize = 4;
};

inline std::ostream& operator<<(std::ostream& os, Message message)
{
    const auto previousSettings = os.setf(std::ios_base::dec);

    os << message.id
       << "\ttoken: "<< static_cast<std::string>(message.token)
       << "\tpayload: " << static_cast<std::string>(message.payload)
       << "\tchecksum: " << std::hex;

    for(const auto& byte : message.calculateCheckSum())
    {
        os.width(2);
        os.fill( '0' );
        os << static_cast<int>(byte);
    }

    os.setf(previousSettings);

    return os;
}
}