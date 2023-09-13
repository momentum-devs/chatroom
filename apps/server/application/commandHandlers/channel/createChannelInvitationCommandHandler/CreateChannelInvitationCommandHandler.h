#pragma once

#include "payloads/CreateChannelInvitationCommandHandlerPayload.h"

namespace server::application
{
class CreateChannelInvitationCommandHandler
{
public:
    virtual ~CreateChannelInvitationCommandHandler() = default;

    virtual void execute(const CreateChannelInvitationCommandHandlerPayload&) const = 0;
};

}
