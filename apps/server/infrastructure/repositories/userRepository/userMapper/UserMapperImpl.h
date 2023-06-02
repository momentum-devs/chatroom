#pragma once

#include "../../../../domain/entities/User.h"
#include "../../../databases/postgres/models/User.h"
#include "UserMapper.h"

namespace server::infrastructure
{
class UserMapperImpl : public UserMapper
{
public:
    domain::User mapToDomainUser(const Models::User&) const override;
};
}
