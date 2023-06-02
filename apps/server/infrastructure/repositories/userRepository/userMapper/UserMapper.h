#pragma once

#include "../../../../domain/entities/User.h"
#include "../../../databases/postgres/models/User.h"

namespace server::infrastructure
{
class UserMapper
{
public:
    virtual ~UserMapper() = default;

    virtual domain::User mapToDomainUser(const Models::User&) const = 0;
};
}
