#pragma once

#include "payloads/LogoutUserCommandHandlerPayload.h"

namespace server::application
{
class LogoutUserCommandHandler
{
public:
    virtual ~LogoutUserCommandHandler() = default;

    virtual void execute(const LogoutUserCommandHandlerPayload&) const = 0;
};
}
