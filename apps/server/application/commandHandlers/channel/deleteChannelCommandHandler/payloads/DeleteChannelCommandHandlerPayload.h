#pragma once

#include <string>

namespace server::application
{
struct DeleteChannelCommandHandlerPayload
{
    const std::string channelId;
    const std::string requesterUserId;
};

inline bool operator==(const DeleteChannelCommandHandlerPayload& lhs, const DeleteChannelCommandHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId and lhs.requesterUserId == rhs.requesterUserId;
}
}
