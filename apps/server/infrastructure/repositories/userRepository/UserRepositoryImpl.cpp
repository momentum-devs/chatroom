#include "UserRepositoryImpl.h"

#include "server/infrastructure/errors/UserRepositoryError.h"
#include "User.odb.h"

namespace server::infrastructure
{
UserRepositoryImpl::UserRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                       std::unique_ptr<UserMapper> userMapperInit)
    : db{std::move(dbInit)}, userMapper{std::move(userMapperInit)}
{
}

domain::User UserRepositoryImpl::createUser(const domain::CreateUserPayload& payload) const
{
    try
    {
        {
            User user{payload.id, payload.email, payload.password, payload.nickname};

            odb::transaction transaction(db->begin());

            db->persist(user);

            transaction.commit();

            return userMapper->mapToDomainUser(user);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<domain::User> UserRepositoryImpl::findUserById(const domain::FindUserByIdPayload& payload) const
{
    try
    {
        typedef odb::query<User> query;

        std::shared_ptr<User> user(db->query_one<User>(query::id == payload.id));

        if (user)
        {
            return userMapper->mapToDomainUser(*user);
        }

        return std::nullopt;
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<domain::User> UserRepositoryImpl::findUserByEmail(const domain::FindUserByEmailPayload& payload) const
{
    try
    {
        typedef odb::query<User> query;

        std::shared_ptr<User> user(db->query_one<User>(query::email == payload.email));

        if (user)
        {
            return userMapper->mapToDomainUser(*user);
        }

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

void UserRepositoryImpl::deleteUser(const domain::DeleteUserPayload& payload) const
{
    try
    {
        odb::transaction t(db->begin());

        db->erase<User>(payload.user.getId());

        t.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}
}
