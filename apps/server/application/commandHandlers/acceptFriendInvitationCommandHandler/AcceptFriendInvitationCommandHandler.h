#pragma once

#include "payloads/AcceptFriendInvitationCommandHandlerPayload.h"

namespace server::application
{
class AcceptFriendInvitationCommandHandler
{
public:
    virtual ~AcceptFriendInvitationCommandHandler() = default;

    virtual void execute(const AcceptFriendInvitationCommandHandlerPayload&) const = 0;
};

}
