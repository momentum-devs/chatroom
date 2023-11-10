#pragma once

#include <string>

namespace server::application
{
struct FindChannelConversationQueryHandlerPayload
{
    const std::string channelId;
};

inline bool operator==(const FindChannelConversationQueryHandlerPayload& lhs, const FindChannelConversationQueryHandlerPayload& rhs)
{
    return lhs.channelId == rhs.channelId;
}
}
