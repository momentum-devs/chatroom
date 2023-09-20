#pragma once

#include "payloads/FindUserQueryHandlerPayload.h"
#include "payloads/FindUserQueryHandlerResult.h"

namespace server::application
{
class FindUserQueryHandler
{
public:
    virtual ~FindUserQueryHandler() = default;

    virtual FindUserQueryHandlerResult execute(const FindUserQueryHandlerPayload&) const = 0;
};

}
