#pragma once

#include <string>

namespace server::application
{
struct FindGroupMessagesQueryHandlerPayload
{
    std::string groupId;
    unsigned offset = 0;
    unsigned limit = 50;
};

inline bool operator==(const FindGroupMessagesQueryHandlerPayload& lhs, const FindGroupMessagesQueryHandlerPayload& rhs)
{
    return lhs.groupId == rhs.groupId && lhs.offset == rhs.offset && lhs.limit == rhs.limit;
}
}
