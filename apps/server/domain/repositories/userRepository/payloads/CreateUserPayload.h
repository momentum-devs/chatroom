#pragma once

#include <optional>
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
    bool emailVerified;
    std::string verificationCode;
    std::optional<std::string> resetPasswordCode;
    std::optional<std::string> avatarUrl;
};

inline bool operator==(const CreateUserPayload& lhs, const CreateUserPayload& rhs)
{
    auto tieStruct = [](const CreateUserPayload& payload)
    {
        return std::tie(payload.id, payload.email, payload.password, payload.nickname, payload.active,
                        payload.emailVerified, payload.verificationCode, payload.resetPasswordCode, payload.avatarUrl);
    };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
