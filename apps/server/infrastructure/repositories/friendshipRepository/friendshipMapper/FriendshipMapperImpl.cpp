#include "FriendshipMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
FriendshipMapperImpl::FriendshipMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                           std::shared_ptr<GroupMapper> groupMapperInit)
    : userMapper{std::move(userMapperInit)}, groupMapper{std::move(groupMapperInit)}
{
}

domain::Friendship FriendshipMapperImpl::mapToDomainFriendship(const Friendship& friendship) const
{
    const auto id = friendship.getId();
    const auto user = userMapper->mapToDomainUser(friendship.getUser());
    const auto userFriend = userMapper->mapToDomainUser(friendship.getUserFriend());
    const auto createdAt = friendship.getCreatedAt();
    const auto group = groupMapper->mapToDomainGroup(friendship.getGroup());

    return domain::Friendship{id, user, userFriend, createdAt, group};
}
}
