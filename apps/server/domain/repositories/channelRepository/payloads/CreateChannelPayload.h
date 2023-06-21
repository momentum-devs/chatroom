#pragma once

#include <string>

namespace server::domain
{
struct CreateChannelPayload
{
    const std::string id;
    const std::string name;
    const std::string creatorId;
};

inline bool operator==(const CreateChannelPayload& lhs, const CreateChannelPayload& rhs)
{
    auto tieStruct = [](const CreateChannelPayload& payload)
    { return std::tie(payload.id, payload.name, payload.creatorId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
