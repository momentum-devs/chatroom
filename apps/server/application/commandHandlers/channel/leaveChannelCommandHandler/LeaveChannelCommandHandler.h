#pragma once

#include "payloads/LeaveChannelCommandHandlerPayload.h"

namespace server::application
{
class LeaveChannelCommandHandler
{
public:
    virtual ~LeaveChannelCommandHandler() = default;

    virtual void execute(const LeaveChannelCommandHandlerPayload&) = 0;
};
}
