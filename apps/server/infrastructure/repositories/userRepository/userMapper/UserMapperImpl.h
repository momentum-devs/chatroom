#pragma once

#include "../../../../domain/entities/User.h"
#include "../../../database/models/User.h"
#include "UserMapper.h"

namespace server::infrastructure
{
class UserMapperImpl : public UserMapper
{
public:
    domain::User mapToDomainUser(const Models::User&) const override;
};
}
