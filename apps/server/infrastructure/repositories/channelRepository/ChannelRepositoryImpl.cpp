#include "ChannelRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <format>

#include "Channel.odb.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"

namespace server::infrastructure
{
ChannelRepositoryImpl::ChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                             std::shared_ptr<ChannelMapper> channelMapperInit)
    : db{std::move(dbInit)}, channelMapper{std::move(channelMapperInit)}
{
}

std::shared_ptr<domain::Channel> ChannelRepositoryImpl::createChannel(const domain::CreateChannelPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto channel =
                std::make_shared<Channel>(payload.id, payload.name, payload.creatorId, currentDate, currentDate);

            odb::transaction transaction(db->begin());

            db->persist(channel);

            transaction.commit();

            return channelMapper->mapToDomainChannel(channel);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}

std::optional<std::shared_ptr<domain::Channel>>
ChannelRepositoryImpl::findChannelById(const domain::FindChannelByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Channel> query;

        std::shared_ptr<Channel> channel(db->query_one<Channel>(query::id == payload.id));

        transaction.commit();

        if (!channel)
        {
            return std::nullopt;
        }

        return channelMapper->mapToDomainChannel(channel);
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}

void ChannelRepositoryImpl::deleteChannel(const domain::DeleteChannelPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Channel>(payload.channel.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}
}
