#pragma once

#include "payloads/RegisterUserCommandHandlerPayload.h"
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
