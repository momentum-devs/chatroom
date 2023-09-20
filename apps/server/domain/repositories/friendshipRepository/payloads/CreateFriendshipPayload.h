#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateFriendshipPayload
{
    const std::string id;
    const std::shared_ptr<User> user;
    const std::shared_ptr<User> userFriend;
};

inline bool operator==(const CreateFriendshipPayload& lhs, const CreateFriendshipPayload& rhs)
{
    auto tieStruct = [](const CreateFriendshipPayload& payload)
    { return std::tie(payload.id, *payload.user, *payload.userFriend); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
