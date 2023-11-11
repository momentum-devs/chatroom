#pragma once

#include "server/application/commandHandlers/message/deleteMessageCommandHandler/payloads/DeleteMessageCommandHandlerPayload.h"

namespace server::application
{
class DeleteMessageCommandHandler
{
public:
    virtual ~DeleteMessageCommandHandler() = default;

    virtual void execute(const DeleteMessageCommandHandlerPayload&) = 0;
};
}
