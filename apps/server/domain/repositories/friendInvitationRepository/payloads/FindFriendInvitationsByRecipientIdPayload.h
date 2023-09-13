#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindFriendInvitationsByRecipientIdPayload
{
    std::string recipientId;
};
}
