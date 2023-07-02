#pragma once

#include <string>

namespace server::application
{
struct AddUserToChannelCommandHandlerPayload
{
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const AddUserToChannelCommandHandlerPayload& lhs,
                       const AddUserToChannelCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const AddUserToChannelCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.channelId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
