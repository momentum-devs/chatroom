#include "ChannelRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include "fmt/format.h"

#include "Channel.odb.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"
#include "server/infrastructure/errors/ResourceNotFoundError.h"

namespace server::infrastructure
{
ChannelRepositoryImpl::ChannelRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                             std::shared_ptr<ChannelMapper> channelMapperInit,
                                             std::shared_ptr<UserMapper> userMapperInit)
    : db{std::move(dbInit)}, channelMapper{std::move(channelMapperInit)}, userMapper{std::move(userMapperInit)}
{
}

std::shared_ptr<domain::Channel> ChannelRepositoryImpl::createChannel(const domain::CreateChannelPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto creator = userMapper->mapToPersistenceUser(payload.creator);

            const auto avatarUrl = payload.avatarUrl ? *payload.avatarUrl : odb::nullable<std::string>();

            const auto channel =
                std::make_shared<Channel>(payload.id, payload.name, creator, currentDate, currentDate, avatarUrl);

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

std::shared_ptr<domain::Channel> ChannelRepositoryImpl::updateChannel(const domain::UpdateChannelPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<Channel> query;

            std::shared_ptr<Channel> channel(db->query_one<Channel>(query::id == payload.channel.getId()));

            if (!channel)
            {
                throw errors::ResourceNotFoundError{
                    fmt::format("Channel with id \"{}\" not found.", payload.channel.getId())};
            }

            channel->setName(payload.channel.getName());

            if (payload.channel.getAvatarUrl())
            {
                channel->setAvatarUrl(*payload.channel.getAvatarUrl());
            }

            db->update(*channel);

            transaction.commit();
        }

        return *findChannelById({payload.channel.getId()});
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
