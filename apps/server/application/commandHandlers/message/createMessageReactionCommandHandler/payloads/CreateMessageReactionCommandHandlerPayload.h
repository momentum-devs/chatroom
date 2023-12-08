#pragma once

#include <string>

namespace server::application
{
struct CreateMessageReactionCommandHandlerPayload
{
    const std::string reactionName;
    const std::string userId;
    const std::string messageId;
};

inline bool operator==(const CreateMessageReactionCommandHandlerPayload& lhs,
                       const CreateMessageReactionCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateMessageReactionCommandHandlerPayload& payload)
    { return std::tie(payload.reactionName, payload.userId, payload.messageId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
