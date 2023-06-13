#pragma once

#include "../../../../domain/entities/User.h"
#include "../../../database/tables/userTable/User.h"
#include "UserMapper.h"

namespace server::infrastructure
{
class UserMapperImpl : public UserMapper
{
public:
    domain::User mapToDomainUser(const User&) const override;
};
}
