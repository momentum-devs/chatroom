#pragma once

#include "payloads/VerifyUserEmailCommandHandlerPayload.h"
#include "payloads/VerifyUserEmailCommandHandlerResult.h"

namespace server::application
{
class VerifyUserEmailCommandHandler
{
public:
    virtual ~VerifyUserEmailCommandHandler() = default;

    virtual VerifyUserEmailCommandHandlerResult execute(const VerifyUserEmailCommandHandlerPayload&) const = 0;
};
}
