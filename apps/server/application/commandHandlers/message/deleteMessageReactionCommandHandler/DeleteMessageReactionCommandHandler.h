#pragma once

#include "payloads/DeleteMessageReactionCommandHandlerPayload.h"

namespace server::application
{
class DeleteMessageReactionCommandHandler
{
public:
    virtual ~DeleteMessageReactionCommandHandler() = default;

    virtual void execute(const DeleteMessageReactionCommandHandlerPayload&) const = 0;
};

}
