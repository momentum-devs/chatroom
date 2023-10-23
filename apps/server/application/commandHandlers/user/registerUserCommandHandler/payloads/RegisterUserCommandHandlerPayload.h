#pragma once

#include <string>

namespace server::application
{
struct RegisterUserCommandHandlerPayload
{
    std::string email;
    std::string password;
    std::string nickname;
};

inline bool operator==(const RegisterUserCommandHandlerPayload& lhs, const RegisterUserCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const RegisterUserCommandHandlerPayload& payload)
    { return std::tie(payload.email, payload.password, payload.nickname); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
