#include "UserChannelRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "server/infrastructure/errors/ResourceNotFoundError.h"
#include "server/infrastructure/errors/UserChannelRepositoryError.h"
#include "UserChannel.odb.h"

namespace server::infrastructure
{
UserChannelRepositoryImpl::UserChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                                     std::unique_ptr<UserChannelMapper> userChannelMapperInit)
    : db{std::move(dbInit)}, userChannelMapper{std::move(userChannelMapperInit)}
{
}

domain::UserChannel UserChannelRepositoryImpl::createUserChannel(const domain::CreateUserChannelPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            UserChannel userChannel{payload.id, payload.userId, payload.channelId, currentDate, currentDate};

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

std::vector<domain::UserChannel>
UserChannelRepositoryImpl::findUsersChannelsByUserId(const domain::FindUsersChannelsByUserIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserChannel> query;

        auto result = db->query<UserChannel>(query::user_id == payload.userId);

        transaction.commit();

        std::vector<domain::UserChannel> domainUserChannels;

        for (auto& userChannel : result)
        {
            domainUserChannels.push_back(userChannelMapper->mapToDomainUserChannel(userChannel));
        }

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

        typedef odb::query<UserChannel> query;

        auto result = db->query<UserChannel>(query::channel_id == payload.channelId);

        transaction.commit();

        std::vector<domain::UserChannel> domainUserChannels;

        for (auto& userChannel : result)
        {
            domainUserChannels.push_back(userChannelMapper->mapToDomainUserChannel(userChannel));
        }

        return domainUserChannels;
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
