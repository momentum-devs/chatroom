#pragma once

#include <string>

namespace server::application
{
struct FindChannelMessagesQueryHandlerPayload
{
    std::string channelId;
    unsigned offset = 0;
    unsigned limit = 50;
};

inline bool operator==(const FindChannelMessagesQueryHandlerPayload& lhs,
                       const FindChannelMessagesQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId && lhs.offset == rhs.offset && lhs.limit == rhs.limit;
}
}
