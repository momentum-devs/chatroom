#include "ChannelInvitationRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "ChannelInvitation.odb.h"
#include "server/infrastructure/errors/ChannelInvitationRepositoryError.h"

namespace server::infrastructure
{
ChannelInvitationRepositoryImpl::ChannelInvitationRepositoryImpl(
    std::shared_ptr<odb::pgsql::database> dbInit, std::shared_ptr<ChannelInvitationMapper> channelInvitationMapperInit,
    std::shared_ptr<UserMapper> userMapperInit, std::shared_ptr<ChannelMapper> channelMapperInit)
    : db{std::move(dbInit)},
      channelInvitationMapper{std::move(channelInvitationMapperInit)},
      userMapper{std::move(userMapperInit)},
      channelMapper{std::move(channelMapperInit)}
{
}

domain::ChannelInvitation
ChannelInvitationRepositoryImpl::createChannelInvitation(const domain::CreateChannelInvitationPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto sender = userMapper->mapToPersistenceUser(payload.sender);

            const auto recipient = userMapper->mapToPersistenceUser(payload.recipient);

            const auto channel = channelMapper->mapToPersistenceChannel(payload.channel);

            ChannelInvitation channelInvitation{payload.id, sender, recipient, channel, currentDate, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(channelInvitation);

            transaction.commit();

            return channelInvitationMapper->mapToDomainChannelInvitation(channelInvitation);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelInvitationRepositoryError{error.what()};
    }
}

std::optional<domain::ChannelInvitation> ChannelInvitationRepositoryImpl::findChannelInvitationById(
    const domain::FindChannelInvitationByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<ChannelInvitation> Query;

        std::shared_ptr<ChannelInvitation> channelInvitation(db->query_one<ChannelInvitation>(Query::id == payload.id));

        transaction.commit();

        if (!channelInvitation)
        {
            return std::nullopt;
        }

        return channelInvitationMapper->mapToDomainChannelInvitation(*channelInvitation);
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelInvitationRepositoryError{error.what()};
    }
}

std::vector<domain::ChannelInvitation> ChannelInvitationRepositoryImpl::findChannelInvitationsByRecipientId(
    const domain::FindChannelInvitationsByRecipientIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<ChannelInvitation> Query;

        auto result = db->query<ChannelInvitation>(Query::recipient->id == payload.recipientId);

        std::vector<domain::ChannelInvitation> domainChannelInvitations;

        for (auto& channelInvitation : result)
        {
            domainChannelInvitations.push_back(
                channelInvitationMapper->mapToDomainChannelInvitation(channelInvitation));
        }

        transaction.commit();

        return domainChannelInvitations;
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelInvitationRepositoryError{error.what()};
    }
}

void ChannelInvitationRepositoryImpl::deleteChannelInvitation(
    const domain::DeleteChannelInvitationPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<ChannelInvitation>(payload.channelInvitation.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelInvitationRepositoryError{error.what()};
    }
}

}
