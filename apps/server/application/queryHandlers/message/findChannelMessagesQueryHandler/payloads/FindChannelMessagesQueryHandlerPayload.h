#pragma once

#include <string>

namespace server::application
{
struct FindChannelMessagesQueryHandlerPayload
{
    const std::string channelId;
};

inline bool operator==(const FindChannelMessagesQueryHandlerPayload& lhs,
                       const FindChannelMessagesQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId;
}
}
