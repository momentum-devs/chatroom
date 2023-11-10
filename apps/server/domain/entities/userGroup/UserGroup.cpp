#include "UserGroup.h"

#include <utility>

namespace server::domain
{
UserGroup::UserGroup(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<Group> groupInit,
                     std::string createdAtInit)
    : id{std::move(idInit)}, createdAt{std::move(createdAtInit)}, user{std::move(userInit)}, group{std::move(groupInit)}
{
}

std::string UserGroup::getId() const
{
    return id;
}

std::shared_ptr<User> UserGroup::getUser() const
{
    return user;
}

std::shared_ptr<Group> UserGroup::getGroup() const
{
    return group;
}

std::string UserGroup::getCreatedAt() const
{
    return createdAt;
}

}
