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
        const auto user = User("uuid", payload.email, payload.password, payload.nickname);

        return userMapper->mapToDomainUser(user);
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<domain::User> UserRepositoryImpl::findUserById(const domain::FindUserByIdPayload&) const
{
    try
    {
        return std::nullopt;
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<domain::User> UserRepositoryImpl::findUserByEmail(const domain::FindUserByEmailPayload&) const
{
    try
    {
        return std::nullopt;
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

void UserRepositoryImpl::updateUser(const domain::UpdateUserPayload&) const
{
    try
    {
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

void UserRepositoryImpl::deleteUser(const domain::DeleteUserPayload&) const
{
    try
    {
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}
}
