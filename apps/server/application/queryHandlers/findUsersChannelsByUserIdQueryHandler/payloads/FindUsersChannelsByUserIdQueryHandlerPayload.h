#pragma once

#include <string>

namespace server::application
{
struct FindUsersChannelsByUserIdQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindUsersChannelsByUserIdQueryHandlerPayload& lhs,
                       const FindUsersChannelsByUserIdQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
