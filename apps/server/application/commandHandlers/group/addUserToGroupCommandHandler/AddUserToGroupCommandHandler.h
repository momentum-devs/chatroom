#pragma once

#include "payloads/AddUserToGroupCommandHandlerPayload.h"

namespace server::application
{
class AddUserToGroupCommandHandler
{
public:
    virtual ~AddUserToGroupCommandHandler() = default;

    virtual void execute(const AddUserToGroupCommandHandlerPayload&) const = 0;
};

}
