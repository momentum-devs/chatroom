#include <boost/date_time/posix_time/posix_time.hpp>

#include "gtest/gtest.h"

#include "../../errors/ResourceNotFoundError.h"
#include "Channel.h"
#include "Channel.odb.h"
#include "DeleteChannelCommandHandlerImpl.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/OperationNotValid.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class DeleteChannelCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"channels\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    User createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        User user{id, email, password, email, false, false, "123", currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    Channel createChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        Channel channel{id, name, creatorId, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    std::unique_ptr<ChannelMapper> channelMapperInit = std::make_unique<ChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, std::move(channelMapperInit));

    DeleteChannelCommandHandlerImpl deleteChannelCommandHandler{channelRepository};
};

TEST_F(DeleteChannelCommandImplIntegrationTest, givenExistingChannelAndRequesterIsChannelCreator_shouldDeleteChannel)
{
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userPassword = faker::Internet::password();

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    deleteChannelCommandHandler.execute({channel.getId(), creatorId});

    typedef odb::query<Channel> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Channel> foundChannel(db->query_one<Channel>(query::id == channelId));

        ASSERT_FALSE(foundChannel);

        transaction.commit();
    }
}

TEST_F(DeleteChannelCommandImplIntegrationTest, givenExistingChannelAndRequesterIsNotChannelCreator_shouldThrow)
{
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userPassword = faker::Internet::password();

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto requesterId = faker::String::uuid();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channel.getId(), requesterId}), errors::OperationNotValid);
}

TEST_F(DeleteChannelCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto channelId = faker::String::uuid();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channelId}), errors::ResourceNotFoundError);
}
