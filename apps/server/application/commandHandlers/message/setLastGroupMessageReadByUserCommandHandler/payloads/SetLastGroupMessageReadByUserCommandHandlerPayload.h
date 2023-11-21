#pragma once

#include <string>

namespace server::application
{
struct SetLastGroupMessageReadByUserCommandHandlerPayload
{
    const std::string messageId;
    const std::string userId;
    const std::string groupId;
};

inline bool operator==(const SetLastGroupMessageReadByUserCommandHandlerPayload& lhs,
                       const SetLastGroupMessageReadByUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const SetLastGroupMessageReadByUserCommandHandlerPayload& payload)
    { return std::tie(payload.messageId, payload.userId, payload.groupId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
