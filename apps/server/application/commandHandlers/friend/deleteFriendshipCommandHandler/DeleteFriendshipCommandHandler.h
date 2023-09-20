#pragma once

#include "payloads/DeleteFriendshipCommandHandlerPayload.h"

namespace server::application
{
class DeleteFriendshipCommandHandler
{
public:
    virtual ~DeleteFriendshipCommandHandler() = default;

    virtual void execute(const DeleteFriendshipCommandHandlerPayload&) const = 0;
};

}
