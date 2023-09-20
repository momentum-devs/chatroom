#pragma once

#include "payloads/FindReceivedFriendInvitationsQueryHandlerPayload.h"
#include "payloads/FindReceivedFriendInvitationsQueryHandlerResult.h"

namespace server::application
{
class FindReceivedFriendInvitationsQueryHandler
{
public:
    virtual ~FindReceivedFriendInvitationsQueryHandler() = default;

    virtual FindReceivedFriendInvitationsQueryHandlerResult
    execute(const FindReceivedFriendInvitationsQueryHandlerPayload&) const = 0;
};

}
