#pragma once

#include <string>

namespace server::application
{
struct CreateUserChannelCommandHandlerPayload
{
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const CreateUserChannelCommandHandlerPayload& lhs,
                       const CreateUserChannelCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateUserChannelCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.channelId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
