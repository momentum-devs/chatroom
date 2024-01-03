#pragma once

#include "payloads/FindMessageReactionsQueryHandlerPayload.h"
#include "payloads/FindMessageReactionsQueryHandlerResult.h"

namespace server::application
{
class FindMessageReactionsQueryHandler
{
public:
    virtual ~FindMessageReactionsQueryHandler() = default;

    virtual FindMessageReactionsQueryHandlerResult execute(const FindMessageReactionsQueryHandlerPayload&) const = 0;
};

}
