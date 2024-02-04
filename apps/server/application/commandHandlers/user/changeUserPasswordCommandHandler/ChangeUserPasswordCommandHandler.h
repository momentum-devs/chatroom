#pragma once

#include "payloads/ChangeUserPasswordCommandHandlerPayload.h"

namespace server::application
{
class ChangeUserPasswordCommandHandler
{
public:
    virtual ~ChangeUserPasswordCommandHandler() = default;

    virtual void execute(const ChangeUserPasswordCommandHandlerPayload&) const = 0;
};
}
