#include "UserMapperImpl.h"

namespace server::infrastructure
{
std::shared_ptr<domain::User> UserMapperImpl::mapToDomainUser(std::shared_ptr<User> user) const
{
    const auto id = user->getId();
    const auto email = user->getEmail();
    const auto password = user->getPassword();
    const auto nickname = user->getNickname();
    const auto active = user->isActive();
    const auto createdAt = user->getCreatedAt();
    const auto updatedAt = user->getUpdatedAt();

    return std::make_shared<domain::User>(id, email, password, nickname, active, createdAt, updatedAt);
}

std::shared_ptr<User> UserMapperImpl::mapToPersistenceUser(std::shared_ptr<domain::User> user) const
{
    const auto id = user->getId();
    const auto email = user->getEmail();
    const auto password = user->getPassword();
    const auto nickname = user->getNickname();
    const auto active = user->isActive();
    const auto createdAt = user->getCreatedAt();
    const auto updatedAt = user->getUpdatedAt();

    return std::make_shared<User>(id, email, password, nickname, active, createdAt, updatedAt);
}
}
