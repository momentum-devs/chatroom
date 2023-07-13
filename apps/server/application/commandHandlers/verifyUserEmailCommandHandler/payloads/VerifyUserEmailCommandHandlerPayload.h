#pragma once

#include <string>

namespace server::application
{
struct VerifyUserEmailCommandHandlerPayload
{
    const std::string email;
    const std::string verificationCode;
};

inline bool operator==(const VerifyUserEmailCommandHandlerPayload& lhs, const VerifyUserEmailCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const VerifyUserEmailCommandHandlerPayload& payload)
    { return std::tie(payload.email, payload.verificationCode); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
