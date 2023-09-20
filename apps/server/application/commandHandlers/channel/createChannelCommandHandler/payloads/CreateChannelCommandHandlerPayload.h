#pragma once

#include <string>

namespace server::application
{
struct CreateChannelCommandHandlerPayload
{
    const std::string name;
    const std::string creatorId;
};

inline bool operator==(const CreateChannelCommandHandlerPayload& lhs, const CreateChannelCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const CreateChannelCommandHandlerPayload& payload)
    { return std::tie(payload.name, payload.creatorId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
