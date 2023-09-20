#pragma once

#include "payloads/CreateFriendshipCommandHandlerPayload.h"

namespace server::application
{
class CreateFriendshipCommandHandler
{
public:
    virtual ~CreateFriendshipCommandHandler() = default;

    virtual void execute(const CreateFriendshipCommandHandlerPayload&) const = 0;
};

}
