#pragma once

#include <string>

namespace server::application
{
struct FindUserFriendshipsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindUserFriendshipsQueryHandlerPayload& lhs,
                       const FindUserFriendshipsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
