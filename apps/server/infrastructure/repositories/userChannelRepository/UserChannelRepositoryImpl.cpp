#include "UserChannelRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include "fmt/format.h"

#include "server/infrastructure/errors/ResourceNotFoundError.h"
#include "server/infrastructure/errors/UserChannelRepositoryError.h"
#include "UserChannel.odb.h"

namespace server::infrastructure
{
UserChannelRepositoryImpl::UserChannelRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                                     std::shared_ptr<UserChannelMapper> userChannelMapperInit,
                                                     std::shared_ptr<UserMapper> userMapperInit,
                                                     std::shared_ptr<ChannelMapper> channelMapperInit)
    : db{std::move(dbInit)},
      userChannelMapper{std::move(userChannelMapperInit)},
      userMapper{std::move(userMapperInit)},
      channelMapper{std::move(channelMapperInit)}
{
}

domain::UserChannel UserChannelRepositoryImpl::createUserChannel(const domain::CreateUserChannelPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto user = userMapper->mapToPersistenceUser(payload.user);

            const auto channel = channelMapper->mapToPersistenceChannel(payload.channel);

            UserChannel userChannel{payload.id, odb::nullable<std::string>(), user, channel, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(userChannel);

            transaction.commit();

            return userChannelMapper->mapToDomainUserChannel(userChannel);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

std::optional<domain::UserChannel>
UserChannelRepositoryImpl::findUserChannel(const domain::FindUserChannelPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserChannel> Query;

        std::shared_ptr<UserChannel> userChannel(
            db->query_one<UserChannel>(Query::user->id == payload.userId && Query::channel->id == payload.channelId));

        transaction.commit();

        if (!userChannel)
        {
            return std::nullopt;
        }

        return userChannelMapper->mapToDomainUserChannel(*userChannel);
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

std::optional<domain::UserChannel>
UserChannelRepositoryImpl::findUserChannelById(const domain::FindUserChannelByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserChannel> Query;

        std::shared_ptr<UserChannel> userChannel(db->query_one<UserChannel>(Query::id == payload.id));

        transaction.commit();

        if (!userChannel)
        {
            return std::nullopt;
        }

        return userChannelMapper->mapToDomainUserChannel(*userChannel);
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

std::vector<domain::UserChannel>
UserChannelRepositoryImpl::findUsersChannelsByUserId(const domain::FindUsersChannelsByUserIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserChannel> Query;

        auto result = db->query<UserChannel>(Query::user->id == payload.userId);

        std::vector<domain::UserChannel> domainUserChannels;

        for (auto& userChannel : result)
        {
            domainUserChannels.push_back(userChannelMapper->mapToDomainUserChannel(userChannel));
        }

        transaction.commit();

        return domainUserChannels;
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

std::vector<domain::UserChannel> UserChannelRepositoryImpl::findUsersChannelsByChannelId(
    const domain::FindUsersChannelsByChannelIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserChannel> Query;

        typedef odb::result<UserChannel> Result;

        Result result = db->query<UserChannel>(Query::channel->id == payload.channelId);

        std::vector<domain::UserChannel> domainUserChannels;

        for (const auto& userChannel : result)
        {
            domainUserChannels.push_back(userChannelMapper->mapToDomainUserChannel(userChannel));
        }

        transaction.commit();

        return domainUserChannels;
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

domain::UserChannel UserChannelRepositoryImpl::updateUserChannel(const domain::UpdateUserChannelPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<UserChannel> query;

            std::shared_ptr<UserChannel> userChannel(
                db->query_one<UserChannel>(query::id == payload.userChannel.getId()));

            if (!userChannel)
            {
                throw errors::ResourceNotFoundError{
                    fmt::format("UserChannel with id \"{}\" not found.", payload.userChannel.getId())};
            }

            const auto lastReadMessageId = payload.userChannel.getLastReadMessageId();

            if (lastReadMessageId)
            {
                userChannel->setLastReadMessageId(*lastReadMessageId);

                db->update(*userChannel);
            }

            transaction.commit();
        }

        return *findUserChannelById({payload.userChannel.getId()});
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

void UserChannelRepositoryImpl::deleteUserChannel(const domain::DeleteUserChannelPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<UserChannel>(payload.userChannel.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::UserChannelRepositoryError{error.what()};
    }
}

}
