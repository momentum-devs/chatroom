#pragma once

#include "payloads/CreateUserChannelCommandHandlerPayload.h"
#include "payloads/CreateUserChannelCommandHandlerResult.h"

namespace server::application
{
class CreateUserChannelCommandHandler
{
public:
    virtual ~CreateUserChannelCommandHandler() = default;

    virtual CreateUserChannelCommandHandlerResult execute(const CreateUserChannelCommandHandlerPayload&) const = 0;
};

}
