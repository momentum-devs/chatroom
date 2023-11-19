#pragma once

#include "payloads/FindChannelMessagesQueryHandlerPayload.h"
#include "payloads/FindChannelMessagesQueryHandlerResult.h"

namespace server::application
{
class FindChannelMessagesQueryHandler
{
public:
    virtual ~FindChannelMessagesQueryHandler() = default;

    virtual FindChannelMessagesQueryHandlerResult
    execute(const FindChannelMessagesQueryHandlerPayload&) const = 0;
};

}
