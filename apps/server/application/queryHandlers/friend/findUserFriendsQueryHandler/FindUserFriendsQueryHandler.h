#pragma once

#include "payloads/FindUserFriendsQueryHandlerPayload.h"
#include "payloads/FindUserFriendsQueryHandlerResult.h"

namespace server::application
{
class FindUserFriendsQueryHandler
{
public:
    virtual ~FindUserFriendsQueryHandler() = default;

    virtual FindUserFriendsQueryHandlerResult
    execute(const FindUserFriendsQueryHandlerPayload&) const = 0;
};

}
