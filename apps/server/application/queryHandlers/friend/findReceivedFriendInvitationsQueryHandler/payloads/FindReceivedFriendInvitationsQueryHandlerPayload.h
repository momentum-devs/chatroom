#pragma once

#include <string>

namespace server::application
{
struct FindReceivedFriendInvitationsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindReceivedFriendInvitationsQueryHandlerPayload& lhs,
                       const FindReceivedFriendInvitationsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
