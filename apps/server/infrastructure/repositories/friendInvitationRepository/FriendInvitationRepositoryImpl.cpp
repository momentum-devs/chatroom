#include "FriendInvitationRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "FriendInvitation.odb.h"
#include "server/infrastructure/errors/FriendInvitationRepositoryError.h"

namespace server::infrastructure
{
FriendInvitationRepositoryImpl::FriendInvitationRepositoryImpl(
    std::shared_ptr<odb::pgsql::database> dbInit, std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit,
    std::shared_ptr<UserMapper> userMapperInit)
    : db{std::move(dbInit)},
      friendInvitationMapper{std::move(friendInvitationMapperInit)},
      userMapper{std::move(userMapperInit)}
{
}

domain::FriendInvitation
FriendInvitationRepositoryImpl::createFriendInvitation(const domain::CreateFriendInvitationPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto sender = userMapper->mapToPersistenceUser(payload.sender);

            const auto recipient = userMapper->mapToPersistenceUser(payload.recipient);

            FriendInvitation friendInvitation{payload.id, sender, recipient, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(friendInvitation);

            transaction.commit();

            return friendInvitationMapper->mapToDomainFriendInvitation(friendInvitation);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::FriendInvitationRepositoryError{error.what()};
    }
}

std::optional<domain::FriendInvitation>
FriendInvitationRepositoryImpl::findFriendInvitationById(const domain::FindFriendInvitationByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<FriendInvitation> Query;

        std::shared_ptr<FriendInvitation> friendInvitation(db->query_one<FriendInvitation>(Query::id == payload.id));

        transaction.commit();

        if (!friendInvitation)
        {
            return std::nullopt;
        }

        return friendInvitationMapper->mapToDomainFriendInvitation(*friendInvitation);
    }
    catch (const std::exception& error)
    {
        throw errors::FriendInvitationRepositoryError{error.what()};
    }
}

std::optional<domain::FriendInvitation>
FriendInvitationRepositoryImpl::findFriendInvitation(const domain::FindFriendInvitationPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<FriendInvitation> Query;

        std::shared_ptr<FriendInvitation> friendInvitation(db->query_one<FriendInvitation>(
            Query::recipient->id == payload.recipientId && Query::sender->id == payload.senderId));

        transaction.commit();

        if (!friendInvitation)
        {
            return std::nullopt;
        }

        return friendInvitationMapper->mapToDomainFriendInvitation(*friendInvitation);
    }
    catch (const std::exception& error)
    {
        throw errors::FriendInvitationRepositoryError{error.what()};
    }
}

std::vector<domain::FriendInvitation> FriendInvitationRepositoryImpl::findFriendInvitationsByRecipientId(
    const domain::FindFriendInvitationsByRecipientIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<FriendInvitation> Query;

        auto result = db->query<FriendInvitation>(Query::recipient->id == payload.recipientId);

        std::vector<domain::FriendInvitation> domainFriendInvitations;

        for (auto& friendInvitation : result)
        {
            domainFriendInvitations.push_back(friendInvitationMapper->mapToDomainFriendInvitation(friendInvitation));
        }

        transaction.commit();

        return domainFriendInvitations;
    }
    catch (const std::exception& error)
    {
        throw errors::FriendInvitationRepositoryError{error.what()};
    }
}

void FriendInvitationRepositoryImpl::deleteFriendInvitation(const domain::DeleteFriendInvitationPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<FriendInvitation>(payload.friendInvitation.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::FriendInvitationRepositoryError{error.what()};
    }
}

}
