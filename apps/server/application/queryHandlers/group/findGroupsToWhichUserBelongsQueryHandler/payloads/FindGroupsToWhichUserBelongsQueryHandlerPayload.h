#pragma once

#include <string>

namespace server::application
{
struct FindGroupsToWhichUserBelongsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindGroupsToWhichUserBelongsQueryHandlerPayload& lhs,
                       const FindGroupsToWhichUserBelongsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
