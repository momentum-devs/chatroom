#include "UserGroupMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
UserGroupMapperImpl::UserGroupMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                         std::shared_ptr<GroupMapper> groupMapperInit)
    : userMapper{std::move(userMapperInit)}, groupMapper{std::move(groupMapperInit)}
{
}

domain::UserGroup UserGroupMapperImpl::mapToDomainUserGroup(const UserGroup& userGroup) const
{
    const auto id = userGroup.getId();
    const auto user = userMapper->mapToDomainUser(userGroup.getUser());
    const auto group = groupMapper->mapToDomainGroup(userGroup.getGroup());
    const auto createdAt = userGroup.getCreatedAt();

    return domain::UserGroup{id, user, group, createdAt};
}
}
