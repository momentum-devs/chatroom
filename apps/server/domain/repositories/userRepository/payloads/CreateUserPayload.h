#pragma once

#include <string>

namespace server::domain
{
struct CreateUserPayload
{
    const std::string id;
    const std::string email;
    const std::string password;
    const std::string nickname;
};

inline bool operator==(const CreateUserPayload& lhs, const CreateUserPayload& rhs)
{
    auto tieStruct = [](const CreateUserPayload& payload)
    { return std::tie(payload.id, payload.email, payload.password, payload.nickname); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
