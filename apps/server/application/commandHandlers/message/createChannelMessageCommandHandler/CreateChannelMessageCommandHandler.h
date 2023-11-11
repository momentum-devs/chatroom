#pragma once

#include "payloads/CreateChannelMessageCommandHandlerPayload.h"
#include "payloads/CreateChannelMessageCommandHandlerResult.h"

namespace server::application
{
class CreateChannelMessageCommandHandler
{
public:
    virtual ~CreateChannelMessageCommandHandler() = default;

    virtual CreateChannelMessageCommandHandlerResult
    execute(const CreateChannelMessageCommandHandlerPayload&) const = 0;
};

}
