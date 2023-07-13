#pragma once

#include <string>

namespace server::application
{
struct SendRegistrationVerificationEmailCommandHandlerPayload
{
    const std::string email;
};

inline bool operator==(const SendRegistrationVerificationEmailCommandHandlerPayload& lhs,
                       const SendRegistrationVerificationEmailCommandHandlerPayload& rhs)
{
    return lhs.email == rhs.email;
}
}
