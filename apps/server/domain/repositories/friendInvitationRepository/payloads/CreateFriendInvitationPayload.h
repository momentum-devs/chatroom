#pragma once

#include <memory>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateFriendInvitationPayload
{
    const std::string id;
    const std::shared_ptr<User> sender;
    const std::shared_ptr<User> recipient;
};

inline bool operator==(const CreateFriendInvitationPayload& lhs, const CreateFriendInvitationPayload& rhs)
{
    auto tieStruct = [](const CreateFriendInvitationPayload& payload)
    { return std::tie(payload.id, *payload.sender, *payload.recipient); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
