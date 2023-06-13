#pragma once

#include "../../../../domain/entities/User.h"
#include "../../../database/tables/userTable/User.h"

namespace server::infrastructure
{
class UserMapper
{
public:
    virtual ~UserMapper() = default;

    virtual domain::User mapToDomainUser(const User&) const = 0;
};
}
