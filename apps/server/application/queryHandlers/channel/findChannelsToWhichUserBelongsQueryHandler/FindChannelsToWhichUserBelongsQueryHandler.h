#pragma once

#include "payloads/FindChannelsToWhichUserBelongsQueryHandlerPayload.h"
#include "payloads/FindChannelsToWhichUserBelongsQueryHandlerResult.h"

namespace server::application
{
class FindChannelsToWhichUserBelongsQueryHandler
{
public:
    virtual ~FindChannelsToWhichUserBelongsQueryHandler() = default;

    virtual FindChannelsToWhichUserBelongsQueryHandlerResult
    execute(const FindChannelsToWhichUserBelongsQueryHandlerPayload&) const = 0;
};

}
