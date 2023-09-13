#pragma once

#include <string>

namespace server::application
{
struct RejectFriendInvitationCommandHandlerPayload
{
    const std::string recipientId;
    const std::string friendInvitationId;
};

inline bool operator==(const RejectFriendInvitationCommandHandlerPayload& lhs,
                       const RejectFriendInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const RejectFriendInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.recipientId, payload.friendInvitationId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
