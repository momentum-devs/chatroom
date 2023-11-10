#pragma once

#include "payloads/FindUsersBelongingToGroupQueryHandlerPayload.h"
#include "payloads/FindUsersBelongingToGroupQueryHandlerResult.h"

namespace server::application
{
class FindUsersBelongingToGroupQueryHandler
{
public:
    virtual ~FindUsersBelongingToGroupQueryHandler() = default;

    virtual FindUsersBelongingToGroupQueryHandlerResult
    execute(const FindUsersBelongingToGroupQueryHandlerPayload&) const = 0;
};

}
