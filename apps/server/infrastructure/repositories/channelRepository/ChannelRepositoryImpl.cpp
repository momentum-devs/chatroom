#include "ChannelRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <format>

#include "Channel.odb.h"
#include "server/infrastructure/errors/ChannelNotFoundError.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"

namespace server::infrastructure
{
ChannelRepositoryImpl::ChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                             std::unique_ptr<ChannelMapper> userMapperInit)
    : db{std::move(dbInit)}, userMapper{std::move(userMapperInit)}
{
}

domain::Channel ChannelRepositoryImpl::createChannel(const domain::CreateChannelPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            Channel user{payload.id, payload.email, payload.password, payload.nickname, currentDate, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(user);

            transaction.commit();

            return userMapper->mapToDomainChannel(user);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}

std::optional<domain::Channel>
ChannelRepositoryImpl::findChannelById(const domain::FindChannelByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Channel> query;

        std::shared_ptr<Channel> user(db->query_one<Channel>(query::id == payload.id));

        transaction.commit();

        if (!user)
        {
            return std::nullopt;
        }

        return userMapper->mapToDomainChannel(*user);
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}

std::optional<domain::Channel>
ChannelRepositoryImpl::findChannelByEmail(const domain::FindChannelByEmailPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Channel> query;

        std::shared_ptr<Channel> user(db->query_one<Channel>(query::email == payload.email));

        transaction.commit();

        if (!user)
        {
            return std::nullopt;
        }

        return userMapper->mapToDomainChannel(*user);
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}

void ChannelRepositoryImpl::updateChannel(const domain::UpdateChannelPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<Channel> query;

            std::shared_ptr<Channel> user(db->query_one<Channel>(query::id == payload.user.getId()));

            if (!user)
            {
                throw errors::ChannelNotFoundError{
                    std::format("Channel with id \"{}\" not found.", payload.user.getId())};
            }

            user->setNickname(payload.user.getNickname());
            user->setPassword(payload.user.getPassword());

            db->update(*user);

            transaction.commit();
        }
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

        db->erase<Channel>(payload.user.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::ChannelRepositoryError{error.what()};
    }
}
}
