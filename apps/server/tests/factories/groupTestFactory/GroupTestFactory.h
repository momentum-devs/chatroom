#pragma once

#include <memory>
#include <optional>

#include "Group.h"
#include "server/domain/entities/group/Group.h"
#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class GroupTestFactory
{
public:
    std::shared_ptr<domain::Group> createDomainGroup();
    std::shared_ptr<infrastructure::Group> createPersistentGroup();
};
}
