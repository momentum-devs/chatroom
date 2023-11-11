#pragma once

#include "payloads/UpdateChannelCommandHandlerPayload.h"
#include "payloads/UpdateChannelCommandHandlerResult.h"

namespace server::application
{
class UpdateChannelCommandHandler
{
public:
    virtual ~UpdateChannelCommandHandler() = default;

    virtual UpdateChannelCommandHandlerResult execute(const UpdateChannelCommandHandlerPayload&) const = 0;
};

}
