#pragma once

#include <string>

namespace server::application
{
struct VerifyUserEmailCommandHandlerPayload
{
    std::string email;
    std::string verificationCode;
};

inline bool operator==(const VerifyUserEmailCommandHandlerPayload& lhs, const VerifyUserEmailCommandHandlerPayload& rhs)
{
    auto tieStruct = [](const VerifyUserEmailCommandHandlerPayload& payload)
    { return std::tie(payload.email, payload.verificationCode); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}
