#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/userGroupRepository/UserGroupRepository.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapper.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class UserGroupRepositoryImpl : public domain::UserGroupRepository
{
public:
    UserGroupRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<UserGroupMapper>,
                            std::shared_ptr<UserMapper>, std::shared_ptr<GroupMapper>);

    domain::UserGroup createUserGroup(const domain::CreateUserGroupPayload&) const;
    std::optional<domain::UserGroup> findUserGroup(const domain::FindUserGroupPayload&) const;
    std::optional<domain::UserGroup> findUserGroupById(const domain::FindUserGroupByIdPayload&) const;
    std::vector<domain::UserGroup> findUsersGroupsByUserId(const domain::FindUsersGroupsByUserIdPayload&) const;
    std::vector<domain::UserGroup> findUsersGroupsByGroupId(const domain::FindUsersGroupsByGroupIdPayload&) const;
    domain::UserGroup updateUserGroup(const domain::UpdateUserGroupPayload&) const;
    void deleteUserGroup(const domain::DeleteUserGroupPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<UserGroupMapper> userGroupMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
