#pragma once

#include "payloads/CreateGroupCommandHandlerResult.h"

namespace server::application
{
class CreateGroupCommandHandler
{
public:
    virtual ~CreateGroupCommandHandler() = default;

    virtual CreateGroupCommandHandlerResult execute() const = 0;
};

}
