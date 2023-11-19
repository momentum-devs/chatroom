#pragma once

#include <string>

namespace server::application
{
struct FindGroupMessagesQueryHandlerPayload
{
    const std::string groupId;
};

inline bool operator==(const FindGroupMessagesQueryHandlerPayload& lhs, const FindGroupMessagesQueryHandlerPayload& rhs)
{
    return lhs.groupId == rhs.groupId;
}
}
