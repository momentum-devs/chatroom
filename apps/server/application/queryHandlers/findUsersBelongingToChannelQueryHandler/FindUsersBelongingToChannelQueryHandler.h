#pragma once

#include "payloads/FindUsersBelongingToChannelQueryHandlerPayload.h"
#include "payloads/FindUsersBelongingToChannelQueryHandlerResult.h"

namespace server::application
{
class FindUsersBelongingToChannelQueryHandler
{
public:
    virtual ~FindUsersBelongingToChannelQueryHandler() = default;

    virtual FindUsersBelongingToChannelQueryHandlerResult
    execute(const FindUsersBelongingToChannelQueryHandlerPayload&) const = 0;
};

}
