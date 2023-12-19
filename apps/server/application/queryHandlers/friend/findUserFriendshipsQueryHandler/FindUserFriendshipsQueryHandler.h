#pragma once

#include "payloads/FindUserFriendshipsQueryHandlerPayload.h"
#include "payloads/FindUserFriendshipsQueryHandlerResult.h"

namespace server::application
{
class FindUserFriendshipsQueryHandler
{
public:
    virtual ~FindUserFriendshipsQueryHandler() = default;

    virtual FindUserFriendshipsQueryHandlerResult execute(const FindUserFriendshipsQueryHandlerPayload&) const = 0;
};

}
