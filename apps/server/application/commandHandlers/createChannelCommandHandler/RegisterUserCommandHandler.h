#pragma once

#include "payloads/CreateChannelCommandHandlerPayload.h"
#include "payloads/RegisterUserCommandHandlerResult.h"

namespace server::application
{
class RegisterUserCommandHandler
{
public:
    virtual ~RegisterUserCommandHandler() = default;

    virtual RegisterUserCommandHandlerResult execute(const RegisterUserCommandHandlerPayload&) const = 0;
};

}
