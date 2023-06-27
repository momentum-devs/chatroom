#pragma once

#include "server/application/commandHandlers/deleteUserChannelCommandHandler/payloads/DeleteUserChannelCommandHandlerPayload.h"

namespace server::application
{
class DeleteUserChannelCommandHandler
{
public:
    virtual ~DeleteUserChannelCommandHandler() = default;

    virtual void execute(const DeleteUserChannelCommandHandlerPayload&) = 0;
};
}
