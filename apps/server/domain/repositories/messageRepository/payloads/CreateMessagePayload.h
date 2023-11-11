#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateMessagePayload
{
    const std::string id;
    const std::string content;
    const std::shared_ptr<User> sender;
    const std::optional<std::shared_ptr<Channel>> channel;
    const std::optional<std::shared_ptr<Group>> group;
};

inline bool operator==(const CreateMessagePayload& lhs, const CreateMessagePayload& rhs)
{
    auto tieStruct = [](const CreateMessagePayload& payload)
    { return std::tie(payload.id, *payload.sender, *payload.group, *payload.channel, payload.content); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
