#pragma once

#include "payloads/RemoveUserFromChannelCommandHandlerPayload.h"

namespace server::application
{
class RemoveUserFromChannelCommandHandler
{
public:
    virtual ~RemoveUserFromChannelCommandHandler() = default;

    virtual void execute(const RemoveUserFromChannelCommandHandlerPayload&) = 0;
};
}
