#pragma once

#include <string>

namespace server::application
{
struct FindUsersBelongingToChannelQueryHandlerPayload
{
    const std::string channelId;
};

inline bool operator==(const FindUsersBelongingToChannelQueryHandlerPayload& lhs,
                       const FindUsersBelongingToChannelQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId;
}
}
