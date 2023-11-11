#pragma once

#include <string>

namespace server::application
{
struct CreateGroupMessageCommandHandlerPayload
{
    const std::string content;
    const std::string senderId;
    const std::string groupId;
};

inline bool operator==(const CreateGroupMessageCommandHandlerPayload& lhs,
                       const CreateGroupMessageCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateGroupMessageCommandHandlerPayload& payload)
    { return std::tie(payload.content, payload.senderId, payload.groupId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
