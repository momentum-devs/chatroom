#pragma once

#include "ChannelInvitationMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class ChannelInvitationMapperImpl : public ChannelInvitationMapper
{
public:
    ChannelInvitationMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::ChannelInvitation mapToDomainChannelInvitation(const ChannelInvitation&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
