#pragma once

#include "payloads/RejectChannelInvitationCommandHandlerPayload.h"

namespace server::application
{
class RejectChannelInvitationCommandHandler
{
public:
    virtual ~RejectChannelInvitationCommandHandler() = default;

    virtual void execute(const RejectChannelInvitationCommandHandlerPayload&) const = 0;
};

}
