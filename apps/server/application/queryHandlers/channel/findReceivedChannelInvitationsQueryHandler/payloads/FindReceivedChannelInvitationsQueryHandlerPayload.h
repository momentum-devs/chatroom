#pragma once

#include <string>

namespace server::application
{
struct FindReceivedChannelInvitationsQueryHandlerPayload
{
    const std::string userId;
};

inline bool operator==(const FindReceivedChannelInvitationsQueryHandlerPayload& lhs,
                       const FindReceivedChannelInvitationsQueryHandlerPayload& rhs)
{
    return lhs.userId == rhs.userId;
}
}
