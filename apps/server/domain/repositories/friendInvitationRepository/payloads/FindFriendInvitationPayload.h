#pragma once

#include <string>

namespace server::domain
{
struct FindFriendInvitationPayload
{
    std::string senderId;
    std::string recipientId;
};
}
