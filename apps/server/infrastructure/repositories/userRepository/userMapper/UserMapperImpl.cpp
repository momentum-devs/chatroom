#include "UserMapperImpl.h"

namespace server::infrastructure
{
domain::User UserMapperImpl::mapToDomainUser(const User& user) const
{
    const auto id = user.getId();
    const auto email = user.getEmail();
    const auto password = user.getPassword();
    const auto nickname = user.getNickname();
    const auto createdAt = user.getCreatedAt();
    const auto updatedAt = user.getUpdatedAt();

    return domain::User{id, email, password, nickname, createdAt, updatedAt};
}
}
