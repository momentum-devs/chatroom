#include "ChannelInvitationMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
ChannelInvitationMapperImpl::ChannelInvitationMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                                         std::shared_ptr<ChannelMapper> channelMapperInit)
    : userMapper{std::move(userMapperInit)}, channelMapper{std::move(channelMapperInit)}
{
}

domain::ChannelInvitation
ChannelInvitationMapperImpl::mapToDomainChannelInvitation(const ChannelInvitation& channelInvitation) const
{
    const auto id = channelInvitation.getId();
    const auto sender = userMapper->mapToDomainUser(channelInvitation.getSender());
    const auto recipient = userMapper->mapToDomainUser(channelInvitation.getRecipient());
    const auto channel = channelMapper->mapToDomainChannel(channelInvitation.getChannel());
    const auto createdAt = channelInvitation.getCreatedAt();

    return domain::ChannelInvitation{id, sender, recipient, channel, createdAt};
}
}
