#include "UserRepositoryImpl.h"

#include "server/infrastructure/errors/UserRepositoryError.h"

namespace server::infrastructure
{
UserRepositoryImpl::UserRepositoryImpl(std::unique_ptr<UserMapper> userMapperInit)
    : userMapper{std::move(userMapperInit)}
{
}

domain::User UserRepositoryImpl::createUser(const domain::CreateUserPayload& payload) const
{
    try
    {
        const auto user = Models::User::create({{"email", QString::fromStdString(payload.email)},
                                                {"password", QString::fromStdString(payload.password)},
                                                {"nickname", QString::fromStdString(payload.nickname)}});

        return userMapper->mapToDomainUser(user);
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<domain::User> UserRepositoryImpl::findUser(const domain::FindUserPayload& payload) const
{
    try
    {
        const auto user = Models::User::firstWhereEq("email", QString::fromStdString(payload.email));

        if (!user)
        {
            return std::nullopt;
        }

        return userMapper->mapToDomainUser(*user);
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

void UserRepositoryImpl::deleteUser(const domain::DeleteUserPayload& payload) const
{
    try
    {
        Models::User::firstWhereEq("id", payload.user.getId())->remove();
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}
}
