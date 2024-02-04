#pragma once

#include <string>

namespace server::application
{
struct SendResetPasswordEmailCommandHandlerPayload
{
    const std::string email;
};

inline bool operator==(const SendResetPasswordEmailCommandHandlerPayload& lhs,
                       const SendResetPasswordEmailCommandHandlerPayload& rhs)
{
    return lhs.email == rhs.email;
}
}
