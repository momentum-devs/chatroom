#pragma once

#include <string>

namespace server::application
{
struct LeaveChannelCommandHandlerPayload
{
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const LeaveChannelCommandHandlerPayload& lhs, const LeaveChannelCommandHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId and lhs.channelId == rhs.channelId;
}
}
