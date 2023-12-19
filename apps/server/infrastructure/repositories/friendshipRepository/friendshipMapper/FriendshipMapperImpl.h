#pragma once

#include "FriendshipMapper.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class FriendshipMapperImpl : public FriendshipMapper
{
public:
    FriendshipMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<GroupMapper>);

    domain::Friendship mapToDomainFriendship(const Friendship&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
