#pragma once

#include <string>

namespace server::application
{
struct RegisterUserCommandHandlerPayload
{
    const std::string email;
    const std::string password;
};

inline bool operator==(const RegisterUserCommandHandlerPayload& lhs, const RegisterUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const RegisterUserCommandHandlerPayload& payload)
    { return std::tie(payload.email, payload.password); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
