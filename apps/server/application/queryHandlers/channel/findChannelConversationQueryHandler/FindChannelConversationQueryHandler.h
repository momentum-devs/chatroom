#pragma once

#include "payloads/FindChannelConversationQueryHandlerPayload.h"
#include "payloads/FindChannelConversationQueryHandlerResult.h"

namespace server::application
{
class FindChannelConversationQueryHandler
{
public:
    virtual ~FindChannelConversationQueryHandler() = default;

    virtual FindChannelConversationQueryHandlerResult execute(const FindChannelConversationQueryHandlerPayload&) const = 0;
};

}
