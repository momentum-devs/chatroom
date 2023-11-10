#pragma once

#include "GroupMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class GroupMapperImpl : public GroupMapper
{
public:
    std::shared_ptr<domain::Group> mapToDomainGroup(std::shared_ptr<Group>) const override;
    std::shared_ptr<Group> mapToPersistenceGroup(std::shared_ptr<domain::Group>) const override;
};
}
