#include "UserRepositoryImpl.h"

#include "server/infrastructure/databases/postgres/models/User.h"

namespace server::infrastructure
{
UserRepositoryImpl::UserRepositoryImpl(std::unique_ptr<UserMapper> userMapperInit) : userMapper{std::move(userMapperInit)} {}

domain::User UserRepositoryImpl::createUser(const domain::CreateUserPayload& payload) const
{
    const auto user = Models::User::create({
        {"email", QString::fromStdString(payload.email)},
        {"password", QString::fromStdString(payload.password)},
        {"nickname", QString::fromStdString(payload.nickname)}
    });

    return userMapper->mapToDomainUser(user);
}

std::optional<domain::User> UserRepositoryImpl::findUser(const domain::FindUserPayload& payload) const
{
    const auto user = Models::User::firstWhereEq("email", QString::fromStdString(payload.email));

    if (!user)
    {
        return std::nullopt;
    }

    return userMapper->mapToDomainUser(*user);
}

void UserRepositoryImpl::deleteUser(const domain::DeleteUserPayload& payload) const
{
    Models::User::firstWhereEq("id", payload.user.getId())->remove();
}
}
