#pragma once

#include <string>

namespace server::application
{
struct DeleteMessageReactionCommandHandlerPayload
{
    const std::string reactionId;
    const std::string userId;
    const std::string messageId;
};

inline bool operator==(const DeleteMessageReactionCommandHandlerPayload& lhs,
                       const DeleteMessageReactionCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const DeleteMessageReactionCommandHandlerPayload& payload)
    { return std::tie(payload.reactionId, payload.userId, payload.messageId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
