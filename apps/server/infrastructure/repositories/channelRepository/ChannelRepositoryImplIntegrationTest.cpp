#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.odb.h"
#include "ChannelRepositoryImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelRepositoryIntegrationTest : public Test
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

    std::shared_ptr<server::infrastructure::UserMapper> userMapper =
        std::make_shared<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<server::infrastructure::ChannelMapper> channelMapper =
        std::make_shared<server::infrastructure::ChannelMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::ChannelRepository> channelRepository =
        std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper, userMapper);
};

TEST_F(ChannelRepositoryIntegrationTest, shouldCreateChannel)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, email, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto channel = channelRepository->createChannel({channelId, name, userMapper->mapToDomainUser(user)});

    ASSERT_EQ(channel->getId(), channelId);
    ASSERT_EQ(channel->getName(), name);
    ASSERT_EQ(channel->getCreator()->getId(), userId);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldDeleteExistingChannel)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = createUser(userId, email, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    Channel channel{channelId, name, user, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();
    }

    const auto domainChannel =
        domain::Channel{channelId, name, userMapper->mapToDomainUser(user), createdAt, updatedAt};

    channelRepository->deleteChannel({domainChannel});

    typedef odb::query<Channel> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Channel> foundChannel(db->query_one<Channel>(query::id == channelId));

        ASSERT_FALSE(foundChannel);

        transaction.commit();
    }
}

TEST_F(ChannelRepositoryIntegrationTest, delete_givenNonExistingChannel_shouldThrowError)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = createUser(userId, email, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    const auto domainChannel =
        domain::Channel{channelId, name, userMapper->mapToDomainUser(user), createdAt, updatedAt};

    ASSERT_ANY_THROW(channelRepository->deleteChannel({domainChannel}));
}

TEST_F(ChannelRepositoryIntegrationTest, shouldFindExistingChannelById)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = createUser(userId, email, password);

    const auto channelId = faker::String::uuid();
    const auto name = faker::Word::noun();

    Channel channel{channelId, name, user, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();
    }

    const auto foundChannel = channelRepository->findChannelById({channelId});

    ASSERT_TRUE(foundChannel);
    ASSERT_EQ(foundChannel->get()->getId(), channelId);
}

TEST_F(ChannelRepositoryIntegrationTest, givenNonExistingChannel_shouldNotFindAnyChannelById)
{
    const auto channelId = faker::String::uuid();

    const auto channel = channelRepository->findChannelById({channelId});

    ASSERT_FALSE(channel);
}
