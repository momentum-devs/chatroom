#pragma once

#include <string>

namespace server::domain
{
struct CreateUserPayload
{
    std::string id;
    std::string email;
    std::string password;
    std::string nickname;
    bool active;
};

inline bool operator==(const CreateUserPayload& lhs, const CreateUserPayload& rhs)
{
    auto tieStruct = [](const CreateUserPayload& payload)
    { return std::tie(payload.id, payload.email, payload.password, payload.nickname, payload.active); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
