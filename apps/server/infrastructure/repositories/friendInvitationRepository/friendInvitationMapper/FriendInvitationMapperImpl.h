#pragma once

#include "FriendInvitationMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class FriendInvitationMapperImpl : public FriendInvitationMapper
{
public:
    explicit FriendInvitationMapperImpl(std::shared_ptr<UserMapper>);

    domain::FriendInvitation mapToDomainFriendInvitation(const FriendInvitation&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
};
}
