#pragma once

#include <string>

namespace server::application
{
struct FindChannelByIdQueryHandlerPayload
{
    const std::string channelId;
};

inline bool operator==(const FindChannelByIdQueryHandlerPayload& lhs, const FindChannelByIdQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId;
}
}
