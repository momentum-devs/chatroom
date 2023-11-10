#include "FriendInvitationMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
FriendInvitationMapperImpl::FriendInvitationMapperImpl(std::shared_ptr<UserMapper> userMapperInit)
    : userMapper{std::move(userMapperInit)}
{
}

domain::FriendInvitation
FriendInvitationMapperImpl::mapToDomainFriendInvitation(const FriendInvitation& channelInvitation) const
{
    const auto id = channelInvitation.getId();
    const auto sender = userMapper->mapToDomainUser(channelInvitation.getSender());
    const auto recipient = userMapper->mapToDomainUser(channelInvitation.getRecipient());
    const auto createdAt = channelInvitation.getCreatedAt();

    return domain::FriendInvitation{id, sender, recipient, createdAt};
}
}
