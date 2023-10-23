#pragma once

#include <string>

namespace server::domain
{
struct FindChannelInvitationPayload
{
    std::string senderId;
    std::string recipientId;
    std::string channelId;
};
}
