#pragma once

#include "payloads/AddUserToChannelCommandHandlerPayload.h"

namespace server::application
{
class AddUserToChannelCommandHandler
{
public:
    virtual ~AddUserToChannelCommandHandler() = default;

    virtual void execute(const AddUserToChannelCommandHandlerPayload&) const = 0;
};

}
