#pragma once

#include <memory>

#include "server/domain/entities/user/User.h"
#include "User.h"

namespace server::tests
{
class UserTestFactory
{
public:
    std::shared_ptr<domain::User> createDomainUser(std::size_t nicknameMaxLength = 12);
    std::shared_ptr<infrastructure::User> createPersistentUser();
};
}
