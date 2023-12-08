#pragma once

#include <string>

namespace server::application
{
struct FindMessageReactionsQueryHandlerPayload
{
    std::string messageId;
};

inline bool operator==(const FindMessageReactionsQueryHandlerPayload& lhs,
                       const FindMessageReactionsQueryHandlerPayload& rhs)
{
    return lhs.messageId == rhs.messageId;
}
}
