#pragma once

#include <string>

#include "server/domain/entities/message/Message.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateReactionPayload
{
    const std::string id;
    const std::string name;
    const std::shared_ptr<User> user;
    const std::shared_ptr<Message> message;
};

inline bool operator==(const CreateReactionPayload& lhs, const CreateReactionPayload& rhs)
{
    auto tieStruct = [](const CreateReactionPayload& payload)
    { return std::tie(payload.id, payload.name, *payload.user, *payload.message); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
