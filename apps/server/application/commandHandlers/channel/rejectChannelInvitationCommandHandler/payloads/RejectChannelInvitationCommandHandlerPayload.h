#pragma once

#include <string>

namespace server::application
{
struct RejectChannelInvitationCommandHandlerPayload
{
    const std::string recipientId;
    const std::string channelInvitationId;
};

inline bool operator==(const RejectChannelInvitationCommandHandlerPayload& lhs,
                       const RejectChannelInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const RejectChannelInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.recipientId, payload.channelInvitationId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
