#pragma once

#include <string>

namespace server::application
{
struct RemoveUserFromChannelCommandHandlerPayload
{
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const RemoveUserFromChannelCommandHandlerPayload& lhs,
                       const RemoveUserFromChannelCommandHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId and lhs.channelId == rhs.channelId;
}
}
