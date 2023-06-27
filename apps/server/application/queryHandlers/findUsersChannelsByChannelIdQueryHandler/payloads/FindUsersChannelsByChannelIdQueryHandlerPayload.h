#pragma once

#include <string>

namespace server::application
{
struct FindUsersChannelsByChannelIdQueryHandlerPayload
{
    const std::string channelId;
};

inline bool operator==(const FindUsersChannelsByChannelIdQueryHandlerPayload& lhs,
                       const FindUsersChannelsByChannelIdQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId;
}
}
