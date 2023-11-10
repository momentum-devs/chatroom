#pragma once

#include "../../../../domain/entities/userGroup/UserGroup.h"
#include "../../../database/entities/UserGroup.h"

namespace server::infrastructure
{
class UserGroupMapper
{
public:
    virtual ~UserGroupMapper() = default;

    virtual domain::UserGroup mapToDomainUserGroup(const UserGroup&) const = 0;
};
}
