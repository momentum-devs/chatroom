#pragma once

#include <optional>
#include <string>

namespace server::application
{
struct UpdateChannelCommandHandlerPayload
{
    std::string id;
    std::string name;
    std::string requesterUserId;
};

inline bool operator==(const UpdateChannelCommandHandlerPayload& lhs, const UpdateChannelCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UpdateChannelCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.name, payload.requesterUserId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
