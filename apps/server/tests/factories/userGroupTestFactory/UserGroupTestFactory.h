#pragma once

#include <memory>

#include "Group.h"
#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"
#include "server/domain/entities/userGroup/UserGroup.h"
#include "User.h"
#include "UserGroup.h"

namespace server::tests
{
class UserGroupTestFactory
{
public:
    std::shared_ptr<domain::UserGroup> createDomainUserGroup(const std::shared_ptr<domain::User>& user,
                                                             const std::shared_ptr<domain::Group>& group);
    std::shared_ptr<infrastructure::UserGroup>
    createPersistentUserGroup(const std::shared_ptr<infrastructure::User>& user,
                              const std::shared_ptr<infrastructure::Group>& group);
};
}
