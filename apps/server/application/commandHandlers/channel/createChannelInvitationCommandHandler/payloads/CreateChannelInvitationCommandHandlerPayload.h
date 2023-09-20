#pragma once

#include <string>

namespace server::application
{
struct CreateChannelInvitationCommandHandlerPayload
{
    const std::string senderId;
    const std::string recipientId;
    const std::string channelId;
};

inline bool operator==(const CreateChannelInvitationCommandHandlerPayload& lhs,
                       const CreateChannelInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateChannelInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.senderId, payload.recipientId, payload.channelId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
