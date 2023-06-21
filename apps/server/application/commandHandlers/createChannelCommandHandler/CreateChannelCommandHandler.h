#pragma once

#include "payloads/CreateChannelCommandHandlerPayload.h"
#include "payloads/CreateChannelCommandHandlerResult.h"

namespace server::application
{
class CreateChannelCommandHandler
{
public:
    virtual ~CreateChannelCommandHandler() = default;

    virtual CreateChannelCommandHandlerResult execute(const CreateChannelCommandHandlerPayload&) const = 0;
};

}
