#pragma once

#include <string>

namespace server::application
{
struct CreateFriendshipCommandHandlerPayload
{
    const std::string userId;
    const std::string userFriendId;
};

inline bool operator==(const CreateFriendshipCommandHandlerPayload& lhs,
                       const CreateFriendshipCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateFriendshipCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.userFriendId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
