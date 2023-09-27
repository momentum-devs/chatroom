#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindConversationByUserAndRecipientIdsPayload
{
    std::string userId;
    std::string recipientId;
};
}
