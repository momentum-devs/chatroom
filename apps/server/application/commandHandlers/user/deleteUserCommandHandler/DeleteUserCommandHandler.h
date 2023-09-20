#pragma once

#include "server/application/commandHandlers/user/deleteUserCommandHandler/payloads/DeleteUserCommandHandlerPayload.h"

namespace server::application
{
class DeleteUserCommandHandler
{
public:
    virtual ~DeleteUserCommandHandler() = default;

    virtual void execute(const DeleteUserCommandHandlerPayload&) = 0;
};
}
