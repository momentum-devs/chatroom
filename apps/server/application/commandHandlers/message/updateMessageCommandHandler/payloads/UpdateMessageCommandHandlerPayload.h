#pragma once

#include <optional>
#include <string>

namespace server::application
{
struct UpdateMessageCommandHandlerPayload
{
    std::string id;
    std::string content;
    std::string requesterUserId;
};

inline bool operator==(const UpdateMessageCommandHandlerPayload& lhs, const UpdateMessageCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UpdateMessageCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.content, payload.requesterUserId); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
