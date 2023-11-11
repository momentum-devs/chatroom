#pragma once

#include "payloads/UpdateMessageCommandHandlerPayload.h"
#include "payloads/UpdateMessageCommandHandlerResult.h"

namespace server::application
{
class UpdateMessageCommandHandler
{
public:
    virtual ~UpdateMessageCommandHandler() = default;

    virtual UpdateMessageCommandHandlerResult execute(const UpdateMessageCommandHandlerPayload&) const = 0;
};

}
