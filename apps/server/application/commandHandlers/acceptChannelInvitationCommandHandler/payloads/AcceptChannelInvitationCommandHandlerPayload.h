#pragma once

#include <string>

namespace server::application
{
struct AcceptChannelInvitationCommandHandlerPayload
{
    const std::string recipientId;
    const std::string channelInvitationId;
};

inline bool operator==(const AcceptChannelInvitationCommandHandlerPayload& lhs,
                       const AcceptChannelInvitationCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const AcceptChannelInvitationCommandHandlerPayload& payload)
    { return std::tie(payload.recipientId, payload.channelInvitationId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
