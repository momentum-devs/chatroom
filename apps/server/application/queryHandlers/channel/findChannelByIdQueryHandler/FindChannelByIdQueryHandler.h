#pragma once

#include "payloads/FindChannelByIdQueryHandlerPayload.h"
#include "payloads/FindChannelByIdQueryHandlerResult.h"

namespace server::application
{
class FindChannelByIdQueryHandler
{
public:
    virtual ~FindChannelByIdQueryHandler() = default;

    virtual FindChannelByIdQueryHandlerResult
    execute(const FindChannelByIdQueryHandlerPayload&) const = 0;
};

}
