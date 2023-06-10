#pragma once

#include <string>

namespace server::application
{
struct LoginUserCommandHandlerPayload
{
    const std::string email;
    const std::string password;
};

inline bool operator==(const LoginUserCommandHandlerPayload& lhs, const LoginUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const LoginUserCommandHandlerPayload& payload)
    { return std::tie(payload.email, payload.password); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
