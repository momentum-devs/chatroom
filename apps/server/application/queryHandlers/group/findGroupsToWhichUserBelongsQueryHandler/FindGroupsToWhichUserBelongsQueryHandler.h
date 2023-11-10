#pragma once

#include "payloads/FindGroupsToWhichUserBelongsQueryHandlerPayload.h"
#include "payloads/FindGroupsToWhichUserBelongsQueryHandlerResult.h"

namespace server::application
{
class FindGroupsToWhichUserBelongsQueryHandler
{
public:
    virtual ~FindGroupsToWhichUserBelongsQueryHandler() = default;

    virtual FindGroupsToWhichUserBelongsQueryHandlerResult
    execute(const FindGroupsToWhichUserBelongsQueryHandlerPayload&) const = 0;
};

}
