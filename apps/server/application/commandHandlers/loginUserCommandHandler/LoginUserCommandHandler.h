#pragma once

#include "payloads/LoginUserCommandHandlerPayload.h"
#include "payloads/LoginUserCommandHandlerResult.h"

namespace server::application
{
class LoginUserCommandHandler
{
public:
    virtual ~LoginUserCommandHandler() = default;

    virtual LoginUserCommandHandlerResult execute(const LoginUserCommandHandlerPayload&) const = 0;
};
}
