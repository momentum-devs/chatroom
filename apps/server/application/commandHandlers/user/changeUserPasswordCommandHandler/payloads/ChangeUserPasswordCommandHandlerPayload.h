#pragma once

#include <string>

namespace server::application
{
struct ChangeUserPasswordCommandHandlerPayload
{
    std::string userId;
    std::string resetPasswordCode;
    std::string oldPassword;
    std::string newPassword;
};

inline bool operator==(const ChangeUserPasswordCommandHandlerPayload& lhs,
                       const ChangeUserPasswordCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const ChangeUserPasswordCommandHandlerPayload& payload)
    { return std::tie(payload.userId, payload.resetPasswordCode, payload.oldPassword, payload.newPassword); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
