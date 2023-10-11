#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "DeleteChannelCommandHandlerImpl.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
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

    std::shared_ptr<User> createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto user = std::make_shared<User>(id, email, password, email, false, false, "123", currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::shared_ptr<Channel> createChannel(const std::string& id, const std::string& name,
                                           const std::shared_ptr<User>& creator)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto channel = std::make_shared<Channel>(id, name, creator, currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

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

    const auto channel = createChannel(channelId, name, user);

    deleteChannelCommandHandler.execute({channel->getId(), user->getId()});

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

    const auto channel = createChannel(channelId, name, user);

    const auto requesterId = faker::String::uuid();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channel->getId(), requesterId}), errors::OperationNotValidError);
}

TEST_F(DeleteChannelCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto requesterUserId = faker::String::uuid();

    const auto channelId = faker::String::uuid();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channelId, requesterUserId}), errors::ResourceNotFoundError);
}
