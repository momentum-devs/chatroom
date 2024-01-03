#pragma once

#include "payloads/CreateGroupMessageCommandHandlerPayload.h"
#include "payloads/CreateGroupMessageCommandHandlerResult.h"

namespace server::application
{
class CreateGroupMessageCommandHandler
{
public:
    virtual ~CreateGroupMessageCommandHandler() = default;

    virtual CreateGroupMessageCommandHandlerResult execute(const CreateGroupMessageCommandHandlerPayload&) const = 0;
};

}
