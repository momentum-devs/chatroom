#pragma once

#include "../../../../domain/entities/channelInvitation/ChannelInvitation.h"
#include "../../../database/entities/ChannelInvitation.h"

namespace server::infrastructure
{
class ChannelInvitationMapper
{
public:
    virtual ~ChannelInvitationMapper() = default;

    virtual domain::ChannelInvitation mapToDomainChannelInvitation(const ChannelInvitation&) const = 0;
};
}
