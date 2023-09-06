#pragma once

#include "payloads/AcceptChannelInvitationCommandHandlerPayload.h"

namespace server::application
{
class AcceptChannelInvitationCommandHandler
{
public:
    virtual ~AcceptChannelInvitationCommandHandler() = default;

    virtual void execute(const AcceptChannelInvitationCommandHandlerPayload&) const = 0;
};

}
