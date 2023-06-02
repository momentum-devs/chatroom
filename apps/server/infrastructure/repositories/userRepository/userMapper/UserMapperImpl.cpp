#include "UserMapperImpl.h"

namespace server::infrastructure
{
domain::User UserMapperImpl::mapToDomainUser(const Models::User& user) const
{
    const auto id = user.getAttributeValue("id").toUInt();
    const auto email = user.getAttributeValue("email").toString().toStdString();
    const auto password = user.getAttributeValue("password").toString().toStdString();
    const auto nickname = user.getAttributeValue("nickname").toString().toStdString();

    return domain::User{id, email, password, nickname};
}
}
