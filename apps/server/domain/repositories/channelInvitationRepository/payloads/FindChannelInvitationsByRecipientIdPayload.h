#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindChannelInvitationsByRecipientIdPayload
{
    std::string recipientId;
};
}
