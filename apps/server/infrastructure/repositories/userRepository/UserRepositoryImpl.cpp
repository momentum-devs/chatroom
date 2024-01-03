#include "UserRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "fmt/format.h"
#include "server/infrastructure/errors/ResourceNotFoundError.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "User.odb.h"

namespace server::infrastructure
{
UserRepositoryImpl::UserRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                       std::shared_ptr<UserMapper> userMapperInit)
    : db{std::move(dbInit)}, userMapper{std::move(userMapperInit)}
{
}

std::shared_ptr<domain::User> UserRepositoryImpl::createUser(const domain::CreateUserPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto avatarUrl = payload.avatarUrl ? *payload.avatarUrl : odb::nullable<std::string>();

            const auto user = std::make_shared<User>(payload.id, payload.email, payload.password, payload.nickname,
                                                     payload.active, payload.emailVerified, payload.verificationCode,
                                                     currentDate, currentDate, avatarUrl);

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

std::optional<std::shared_ptr<domain::User>>
UserRepositoryImpl::findUserById(const domain::FindUserByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<User> query;

        std::shared_ptr<User> user(db->query_one<User>(query::id == payload.id));

        transaction.commit();

        if (!user)
        {
            return std::nullopt;
        }

        return userMapper->mapToDomainUser(user);
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::optional<std::shared_ptr<domain::User>>
UserRepositoryImpl::findUserByEmail(const domain::FindUserByEmailPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<User> query;

        std::shared_ptr<User> user(db->query_one<User>(query::email == payload.email));

        transaction.commit();

        if (!user)
        {
            return std::nullopt;
        }

        return userMapper->mapToDomainUser(user);
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}

std::shared_ptr<domain::User> UserRepositoryImpl::updateUser(const domain::UpdateUserPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<User> query;

            std::shared_ptr<User> user(db->query_one<User>(query::id == payload.user.getId()));

            if (!user)
            {
                throw errors::ResourceNotFoundError{
                    fmt::format("User with id \"{}\" not found.", payload.user.getId())};
            }

            user->setNickname(payload.user.getNickname());
            user->setPassword(payload.user.getPassword());
            user->setActive(payload.user.isActive());
            user->setEmailVerified(payload.user.isEmailVerified());
            user->setVerificationCode(payload.user.getVerificationCode());

            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());
            
            user->setUpdatedAt(currentDate);

            if (payload.user.getAvatarUrl())
            {
                user->setAvatarUrl(*payload.user.getAvatarUrl());
            }

            db->update(*user);

            transaction.commit();
        }

        return *findUserById({payload.user.getId()});
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
        odb::transaction transaction(db->begin());

        db->erase<User>(payload.user.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::UserRepositoryError{error.what()};
    }
}
}
