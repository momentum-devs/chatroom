#pragma once

#include <string>
#include <vector>

#include "../../entities/group/Group.h"
#include "payloads/CreateUserGroupPayload.h"
#include "payloads/DeleteUserGroupPayload.h"
#include "payloads/FindUserGroupByIdPayload.h"
#include "payloads/FindUsersGroupsByGroupIdPayload.h"
#include "payloads/FindUsersGroupsByUserIdPayload.h"
#include "payloads/UpdateUserGroupPayload.h"
#include "server/domain/repositories/userGroupRepository/payloads/FindUserGroupPayload.h"

namespace server::domain
{
class UserGroupRepository
{
public:
    virtual ~UserGroupRepository() = default;

    virtual UserGroup createUserGroup(const CreateUserGroupPayload&) const = 0;
    virtual std::optional<domain::UserGroup> findUserGroup(const FindUserGroupPayload&) const = 0;
    virtual std::optional<UserGroup> findUserGroupById(const FindUserGroupByIdPayload&) const = 0;
    virtual std::vector<UserGroup> findUsersGroupsByUserId(const FindUsersGroupsByUserIdPayload&) const = 0;
    virtual std::vector<UserGroup> findUsersGroupsByGroupId(const FindUsersGroupsByGroupIdPayload&) const = 0;
    virtual UserGroup updateUserGroup(const UpdateUserGroupPayload&) const = 0;
    virtual void deleteUserGroup(const DeleteUserGroupPayload&) const = 0;
};
}
