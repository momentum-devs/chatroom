#pragma once

#include <string>

namespace server::application
{
struct DeleteMessageCommandHandlerPayload
{
    const std::string messageId;
    const std::string requesterUserId;
};

inline bool operator==(const DeleteMessageCommandHandlerPayload& lhs, const DeleteMessageCommandHandlerPayload& rhs)
{
    return lhs.messageId == rhs.messageId and lhs.requesterUserId == rhs.requesterUserId;
}
}
