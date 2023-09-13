#pragma once

#include <string>

namespace server::application
{
struct AcceptFriendInvitationCommandHandlerPayload
{
    const std::string recipientId;
    const std::string friendInvitationId;
};

inline bool operator==(const AcceptFriendInvitationCommandHandlerPayload& lhs,
                       const AcceptFriendInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const AcceptFriendInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.recipientId, payload.friendInvitationId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
