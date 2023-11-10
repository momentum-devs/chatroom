#pragma once

#include <string>

#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateUserGroupPayload
{
    const std::string id;
    const std::shared_ptr<User> user;
    const std::shared_ptr<Group> group;
};

inline bool operator==(const CreateUserGroupPayload& lhs, const CreateUserGroupPayload& rhs)
{
    auto tieStruct = [](const CreateUserGroupPayload& payload)
    { return std::tie(payload.id, *payload.user, *payload.group); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
