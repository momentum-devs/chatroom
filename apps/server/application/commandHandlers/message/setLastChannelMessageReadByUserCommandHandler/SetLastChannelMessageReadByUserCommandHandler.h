#pragma once

#include "payloads/SetLastChannelMessageReadByUserCommandHandlerPayload.h"

namespace server::application
{
class SetLastChannelMessageReadByUserCommandHandler
{
public:
    virtual ~SetLastChannelMessageReadByUserCommandHandler() = default;

    virtual void execute(const SetLastChannelMessageReadByUserCommandHandlerPayload&) const = 0;
};

}
