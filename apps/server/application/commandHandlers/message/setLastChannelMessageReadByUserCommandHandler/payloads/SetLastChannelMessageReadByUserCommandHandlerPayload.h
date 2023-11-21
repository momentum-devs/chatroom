#pragma once

#include <string>

namespace server::application
{
struct SetLastChannelMessageReadByUserCommandHandlerPayload
{
    const std::string messageId;
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const SetLastChannelMessageReadByUserCommandHandlerPayload& lhs,
                       const SetLastChannelMessageReadByUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const SetLastChannelMessageReadByUserCommandHandlerPayload& payload)
    { return std::tie(payload.messageId, payload.userId, payload.channelId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
