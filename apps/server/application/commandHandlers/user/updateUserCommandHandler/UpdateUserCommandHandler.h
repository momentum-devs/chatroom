#pragma once

#include "payloads/UpdateUserCommandHandlerPayload.h"
#include "payloads/UpdateUserCommandHandlerResult.h"

namespace server::application
{
class UpdateUserCommandHandler
{
public:
    virtual ~UpdateUserCommandHandler() = default;

    virtual UpdateUserCommandHandlerResult execute(const UpdateUserCommandHandlerPayload&) const = 0;
};

}
