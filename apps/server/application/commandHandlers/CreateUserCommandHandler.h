#pragma once

#include "payloads/CreateUserCommandHandlerPayload.h"
#include "payloads/CreateUserCommandHandlerResult.h"

namespace server::application
{
class CreateUserCommandHandler
{
public:
    virtual ~CreateUserCommandHandler() = default;

    virtual CreateUserCommandHandlerResult execute(const CreateUserCommandHandlerPayload&) const = 0;
};

}
