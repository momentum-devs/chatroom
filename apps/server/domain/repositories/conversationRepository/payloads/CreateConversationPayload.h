#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/channel/Channel.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
struct CreateConversationPayload
{
    const std::string id;
    const std::optional<std::shared_ptr<User>> user;
    const std::optional<std::shared_ptr<User>> recipient;
    const std::optional<std::shared_ptr<Channel>> channel;
};

inline bool operator==(const CreateConversationPayload& lhs, const CreateConversationPayload& rhs)
{
    auto tieStruct = [](const CreateConversationPayload& payload)
    { return std::tie(payload.id, *payload.user, *payload.recipient, *payload.channel); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
