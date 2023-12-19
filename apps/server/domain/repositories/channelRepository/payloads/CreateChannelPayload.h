#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateChannelPayload
{
    const std::string id;
    const std::string name;
    const std::shared_ptr<User> creator;
    const std::optional<std::string> avatarUrl;
};

inline bool operator==(const CreateChannelPayload& lhs, const CreateChannelPayload& rhs)
{
    auto tieStruct = [](const CreateChannelPayload& payload)
    { return std::tie(payload.id, payload.name, payload.creator, payload.avatarUrl); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
