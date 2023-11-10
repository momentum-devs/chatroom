#pragma once

#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"
#include "UserGroupMapper.h"

namespace server::infrastructure
{
class UserGroupMapperImpl : public UserGroupMapper
{
public:
    UserGroupMapperImpl(std::shared_ptr<UserMapper>, std::shared_ptr<GroupMapper>);

    domain::UserGroup mapToDomainUserGroup(const UserGroup&) const override;

private:
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
