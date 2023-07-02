#pragma once

#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateUserChannelPayload
{
    const std::string id;
    const std::shared_ptr<User> user;
    const std::shared_ptr<Channel> channel;
};

inline bool operator==(const CreateUserChannelPayload& lhs, const CreateUserChannelPayload& rhs)
{
    auto tieStruct = [](const CreateUserChannelPayload& payload)
    { return std::tie(payload.id, *payload.user, *payload.channel); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
