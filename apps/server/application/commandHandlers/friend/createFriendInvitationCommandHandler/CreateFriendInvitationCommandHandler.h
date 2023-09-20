#pragma once

#include "payloads/CreateFriendInvitationCommandHandlerPayload.h"

namespace server::application
{
class CreateFriendInvitationCommandHandler
{
public:
    virtual ~CreateFriendInvitationCommandHandler() = default;

    virtual void execute(const CreateFriendInvitationCommandHandlerPayload&) const = 0;
};

}
