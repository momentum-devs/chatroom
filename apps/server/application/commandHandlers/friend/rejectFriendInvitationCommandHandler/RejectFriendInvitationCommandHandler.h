#pragma once

#include "payloads/RejectFriendInvitationCommandHandlerPayload.h"

namespace server::application
{
class RejectFriendInvitationCommandHandler
{
public:
    virtual ~RejectFriendInvitationCommandHandler() = default;

    virtual void execute(const RejectFriendInvitationCommandHandlerPayload&) const = 0;
};

}
