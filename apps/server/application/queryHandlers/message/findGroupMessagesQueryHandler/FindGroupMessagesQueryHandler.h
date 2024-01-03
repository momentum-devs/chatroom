#pragma once

#include "payloads/FindGroupMessagesQueryHandlerPayload.h"
#include "payloads/FindGroupMessagesQueryHandlerResult.h"

namespace server::application
{
class FindGroupMessagesQueryHandler
{
public:
    virtual ~FindGroupMessagesQueryHandler() = default;

    virtual FindGroupMessagesQueryHandlerResult execute(const FindGroupMessagesQueryHandlerPayload&) const = 0;
};

}
