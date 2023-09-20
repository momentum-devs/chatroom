#include "FriendshipRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Friendship.odb.h"
#include "server/infrastructure/errors/FriendshipRepositoryError.h"

namespace server::infrastructure
{
FriendshipRepositoryImpl::FriendshipRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                                   std::shared_ptr<FriendshipMapper> friendshipMapperInit,
                                                   std::shared_ptr<UserMapper> userMapperInit)
    : db{std::move(dbInit)}, friendshipMapper{std::move(friendshipMapperInit)}, userMapper{std::move(userMapperInit)}
{
}

domain::Friendship FriendshipRepositoryImpl::createFriendship(const domain::CreateFriendshipPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto user = userMapper->mapToPersistenceUser(payload.user);

            const auto userFriend = userMapper->mapToPersistenceUser(payload.userFriend);

            Friendship friendship{payload.id, user, userFriend, currentDate, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(friendship);

            transaction.commit();

            return friendshipMapper->mapToDomainFriendship(friendship);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::FriendshipRepositoryError{error.what()};
    }
}

std::optional<domain::Friendship>
FriendshipRepositoryImpl::findFriendshipById(const domain::FindFriendshipByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Friendship> Query;

        std::shared_ptr<Friendship> friendship(db->query_one<Friendship>(Query::id == payload.id));

        transaction.commit();

        if (!friendship)
        {
            return std::nullopt;
        }

        return friendshipMapper->mapToDomainFriendship(*friendship);
    }
    catch (const std::exception& error)
    {
        throw errors::FriendshipRepositoryError{error.what()};
    }
}

std::vector<domain::Friendship>
FriendshipRepositoryImpl::findFriendshipsByUserId(const domain::FindFriendshipsByUserIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Friendship> Query;

        auto result =
            db->query<Friendship>(Query::user->id == payload.userId || Query::user_friend->id == payload.userId);

        std::vector<domain::Friendship> domainFriendships;

        for (auto& friendship : result)
        {
            domainFriendships.push_back(friendshipMapper->mapToDomainFriendship(friendship));
        }

        transaction.commit();

        return domainFriendships;
    }
    catch (const std::exception& error)
    {
        throw errors::FriendshipRepositoryError{error.what()};
    }
}

std::optional<domain::Friendship>
FriendshipRepositoryImpl::findFriendshipByUserIds(const domain::FindFriendshipByUserIdsPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Friendship> Query;

        std::shared_ptr<Friendship> friendship(db->query_one<Friendship>(
            Query::user->id == payload.userId && Query::user_friend->id == payload.userFriendId));

        if (!friendship)
        {
            friendship = db->query_one<Friendship>(Query::user->id == payload.userFriendId &&
                                                   Query::user_friend->id == payload.userId);
        }

        transaction.commit();

        if (!friendship)
        {
            return std::nullopt;
        }

        return friendshipMapper->mapToDomainFriendship(*friendship);
    }
    catch (const std::exception& error)
    {
        throw errors::FriendshipRepositoryError{error.what()};
    }
}

void FriendshipRepositoryImpl::deleteFriendship(const domain::DeleteFriendshipPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Friendship>(payload.friendship.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::FriendshipRepositoryError{error.what()};
    }
}

}
