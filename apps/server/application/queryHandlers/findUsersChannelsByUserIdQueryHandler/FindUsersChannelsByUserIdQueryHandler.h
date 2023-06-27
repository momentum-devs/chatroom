#pragma once

#include "payloads/FindUsersChannelsByUserIdQueryHandlerPayload.h"
#include "payloads/FindUsersChannelsByUserIdQueryHandlerResult.h"

namespace server::application
{
class FindUsersChannelsByUserIdQueryHandler
{
public:
    virtual ~FindUsersChannelsByUserIdQueryHandler() = default;

    virtual FindUsersChannelsByUserIdQueryHandlerResult
    execute(const FindUsersChannelsByUserIdQueryHandlerPayload&) const = 0;
};

}
