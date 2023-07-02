#pragma once

#include "../../../../domain/entities/user/User.h"
#include "../../../database/entities/User.h"

namespace server::infrastructure
{
class UserMapper
{
public:
    virtual ~UserMapper() = default;

    virtual std::shared_ptr<domain::User> mapToDomainUser(const std::shared_ptr<User>) const = 0;
    virtual std::shared_ptr<User> mapToPersistenceUser(const std::shared_ptr<domain::User>) const = 0;
};
}
