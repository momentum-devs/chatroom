#pragma once

#include <optional>
#include <string>

namespace server::application
{
struct UpdateUserCommandHandlerPayload
{
    std::string id;
    std::optional<std::string> nickname;
    std::optional<std::string> password;
};

inline bool operator==(const UpdateUserCommandHandlerPayload& lhs, const UpdateUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const UpdateUserCommandHandlerPayload& payload)
    { return std::tie(payload.id, payload.nickname, payload.password); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
