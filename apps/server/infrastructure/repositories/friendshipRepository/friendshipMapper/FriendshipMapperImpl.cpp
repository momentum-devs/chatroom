#include "FriendshipMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
FriendshipMapperImpl::FriendshipMapperImpl(std::shared_ptr<UserMapper> userMapperInit)
    : userMapper{std::move(userMapperInit)}
{
}

domain::Friendship FriendshipMapperImpl::mapToDomainFriendship(const Friendship& channelInvitation) const
{
    const auto id = channelInvitation.getId();
    const auto user = userMapper->mapToDomainUser(channelInvitation.getUser());
    const auto userFriend = userMapper->mapToDomainUser(channelInvitation.getUserFriend());
    const auto createdAt = channelInvitation.getCreatedAt();

    return domain::Friendship{id, user, userFriend, createdAt};
}
}
