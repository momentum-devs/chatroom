#pragma once

#include "payloads/FindUsersChannelsByChannelIdQueryHandlerPayload.h"
#include "payloads/FindUsersChannelsByChannelIdQueryHandlerResult.h"

namespace server::application
{
class FindUsersChannelsByChannelIdQueryHandler
{
public:
    virtual ~FindUsersChannelsByChannelIdQueryHandler() = default;

    virtual FindUsersChannelsByChannelIdQueryHandlerResult
    execute(const FindUsersChannelsByChannelIdQueryHandlerPayload&) const = 0;
};

}
