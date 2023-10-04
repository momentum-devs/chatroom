#pragma once

#include "payloads/FindReceivedChannelInvitationsQueryHandlerPayload.h"
#include "payloads/FindReceivedChannelInvitationsQueryHandlerResult.h"

namespace server::application
{
class FindReceivedChannelInvitationsQueryHandler
{
public:
    virtual ~FindReceivedChannelInvitationsQueryHandler() = default;

    virtual FindReceivedChannelInvitationsQueryHandlerResult
    execute(const FindReceivedChannelInvitationsQueryHandlerPayload&) const = 0;
};

}
