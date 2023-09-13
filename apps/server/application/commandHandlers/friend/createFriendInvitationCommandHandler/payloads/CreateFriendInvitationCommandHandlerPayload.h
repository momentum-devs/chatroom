#pragma once

#include <string>

namespace server::application
{
struct CreateFriendInvitationCommandHandlerPayload
{
    const std::string senderId;
    const std::string recipientId;
};

inline bool operator==(const CreateFriendInvitationCommandHandlerPayload& lhs,
                       const CreateFriendInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateFriendInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.senderId, payload.recipientId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
