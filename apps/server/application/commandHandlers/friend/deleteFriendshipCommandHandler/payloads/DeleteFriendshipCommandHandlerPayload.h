#pragma once

#include <string>

namespace server::application
{
struct DeleteFriendshipCommandHandlerPayload
{
    const std::string userId;
    const std::string userFriendId;
};

inline bool operator==(const DeleteFriendshipCommandHandlerPayload& lhs,
                       const DeleteFriendshipCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const DeleteFriendshipCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.userFriendId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
