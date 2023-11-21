#pragma once

#include "payloads/SetLastGroupMessageReadByUserCommandHandlerPayload.h"

namespace server::application
{
class SetLastGroupMessageReadByUserCommandHandler
{
public:
    virtual ~SetLastGroupMessageReadByUserCommandHandler() = default;

    virtual void execute(const SetLastGroupMessageReadByUserCommandHandlerPayload&) const = 0;
};

}
