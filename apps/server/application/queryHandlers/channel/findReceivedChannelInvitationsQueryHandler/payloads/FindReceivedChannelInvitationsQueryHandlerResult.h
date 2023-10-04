#pragma once

#include <vector>

#include "../../../../../domain/entities/channelInvitation/ChannelInvitation.h"

namespace server::application
{
struct FindReceivedChannelInvitationsQueryHandlerResult
{
    std::vector<domain::ChannelInvitation> channelInvitations;
};
}
