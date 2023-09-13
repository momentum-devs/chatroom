#pragma once

#include "server/application/commandHandlers/channel/deleteChannelCommandHandler/payloads/DeleteChannelCommandHandlerPayload.h"

namespace server::application
{
class DeleteChannelCommandHandler
{
public:
    virtual ~DeleteChannelCommandHandler() = default;

    virtual void execute(const DeleteChannelCommandHandlerPayload&) = 0;
};
}
