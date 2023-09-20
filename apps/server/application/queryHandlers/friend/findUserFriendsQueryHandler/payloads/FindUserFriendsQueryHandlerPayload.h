#pragma once

#include <string>

namespace server::application
{
struct FindUserFriendsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindUserFriendsQueryHandlerPayload& lhs,
                       const FindUserFriendsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
