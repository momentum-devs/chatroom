#pragma once

#include "payloads/SendRegistrationVerificationEmailCommandHandlerPayload.h"

namespace server::application
{
class SendRegistrationVerificationEmailCommandHandler
{
public:
    virtual ~SendRegistrationVerificationEmailCommandHandler() = default;

    virtual void execute(const SendRegistrationVerificationEmailCommandHandlerPayload&) const = 0;
};
}
