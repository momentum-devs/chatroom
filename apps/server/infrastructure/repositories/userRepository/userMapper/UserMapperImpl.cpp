#include "UserMapperImpl.h"

namespace server::infrastructure
{
domain::User UserMapperImpl::mapToDomainUser(const User& user) const
{
    const auto id = 0;
    const auto email = user.getEmail();
    const auto password = user.getPassword();
    const auto nickname = user.getNickname();

    return domain::User{id, email, password, nickname};
}
}
