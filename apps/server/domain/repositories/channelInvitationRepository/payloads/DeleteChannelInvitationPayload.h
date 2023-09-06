#pragma once

#include <string>

#include "../../../entities/channelInvitation/ChannelInvitation.h"

namespace server::domain
{
struct DeleteChannelInvitationPayload
{
    ChannelInvitation channelInvitation;
};
}
