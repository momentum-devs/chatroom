#pragma once

#include <string>

namespace server::domain
{
struct CreateUserChannelPayload
{
    const std::string id;
    const std::string userId;
    const std::string channelId;
};

inline bool operator==(const CreateUserChannelPayload& lhs, const CreateUserChannelPayload& rhs)
{
    auto tieStruct = [](const CreateUserChannelPayload& payload)
    { return std::tie(payload.id, payload.name, payload.creatorId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
