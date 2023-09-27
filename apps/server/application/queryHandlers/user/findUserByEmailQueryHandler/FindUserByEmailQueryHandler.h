#pragma once

#include "payloads/FindUserByEmailQueryHandlerPayload.h"
#include "payloads/FindUserByEmailQueryHandlerResult.h"

namespace server::application
{
class FindUserByEmailQueryHandler
{
public:
    virtual ~FindUserByEmailQueryHandler() = default;

    virtual FindUserByEmailQueryHandlerResult execute(const FindUserByEmailQueryHandlerPayload&) const = 0;
};

}
