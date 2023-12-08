#pragma once

#include "payloads/CreateMessageReactionCommandHandlerPayload.h"
#include "payloads/CreateMessageReactionCommandHandlerResult.h"

namespace server::application
{
class CreateMessageReactionCommandHandler
{
public:
    virtual ~CreateMessageReactionCommandHandler() = default;

    virtual CreateMessageReactionCommandHandlerResult
    execute(const CreateMessageReactionCommandHandlerPayload&) const = 0;
};

}
