#pragma once

#include "../../../../domain/entities/group/Group.h"
#include "../../../database/entities/Group.h"

namespace server::infrastructure
{
class GroupMapper
{
public:
    virtual ~GroupMapper() = default;

    virtual std::shared_ptr<domain::Group> mapToDomainGroup(std::shared_ptr<Group>) const = 0;
    virtual std::shared_ptr<Group> mapToPersistenceGroup(std::shared_ptr<domain::Group>) const = 0;
};
}
