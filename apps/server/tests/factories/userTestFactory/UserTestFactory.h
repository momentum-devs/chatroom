#pragma once

#include <memory>

#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class UserTestFactory
{
public:
    static std::shared_ptr<domain::User> createDomainUser();
    static std::shared_ptr<infrastructure::User> createPersistentUser();
};
}
