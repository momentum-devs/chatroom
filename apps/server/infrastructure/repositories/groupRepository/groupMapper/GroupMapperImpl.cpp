#include "GroupMapperImpl.h"

namespace server::infrastructure
{
std::shared_ptr<domain::Group> GroupMapperImpl::mapToDomainGroup(std::shared_ptr<Group> group) const
{
    const auto id = group->getId();

    const auto createdAt = group->getCreatedAt();

    return std::make_shared<domain::Group>(id, createdAt);
}

std::shared_ptr<Group> GroupMapperImpl::mapToPersistenceGroup(std::shared_ptr<domain::Group> group) const
{
    const auto id = group->getId();

    const auto createdAt = group->getCreatedAt();

    return std::make_shared<Group>(id, createdAt);
}
}
