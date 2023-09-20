#pragma once

#include "FriendshipMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class FriendshipMapperImpl : public FriendshipMapper
{
public:
    explicit FriendshipMapperImpl(std::shared_ptr<UserMapper>);

    domain::Friendship mapToDomainFriendship(const Friendship&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
};
}
