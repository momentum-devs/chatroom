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
        auto checksum = bytes::Bytes{0,0,0,0};

        auto messageBody = bytes::Bytes{static_cast<unsigned char>(id)} + token + payload;

        for(u_int32_t i = 0; i < messageBody.size(); ++i)
        {
            checksum[i % checksumSize] += messageBody[i];
        }

        return checksum;
    }

    inline u_int32_t size() const
    {
        return sizeof id + tokenSize + payload.size() + checksumSize;
    }

    inline static constexpr u_int32_t idSize = 1;
    inline static constexpr u_int32_t checksumSize = 4;
    inline static constexpr u_int32_t tokenSize = 40;
};

inline std::ostream& operator<<(std::ostream& os, const Message& message)
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

inline bool operator==(const Message& lhs, const Message& rhs)
{
    auto tieStruct = [](const Message& message)
    {
        return std::tie(message.id, message.token, message.payload);
    };

    return tieStruct(lhs) == tieStruct(rhs);
}
}