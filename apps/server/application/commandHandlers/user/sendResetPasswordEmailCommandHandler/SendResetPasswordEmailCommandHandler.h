#pragma once

#include "payloads/SendResetPasswordEmailCommandHandlerPayload.h"

namespace server::application
{
class SendResetPasswordEmailCommandHandler
{
public:
    virtual ~SendResetPasswordEmailCommandHandler() = default;

    virtual void execute(const SendResetPasswordEmailCommandHandlerPayload&) const = 0;
};
}
