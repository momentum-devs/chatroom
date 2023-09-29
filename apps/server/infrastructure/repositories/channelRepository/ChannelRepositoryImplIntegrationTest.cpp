#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.odb.h"
#include "ChannelRepositoryImpl.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"
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
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto creatorId = user->getId();

    const auto channel = channelRepository->createChannel({id, name, userMapper->mapToDomainUser(user)});

    ASSERT_EQ(channel->getId(), id);
    ASSERT_EQ(channel->getName(), name);
    ASSERT_EQ(channel->getCreator()->getId(), userId);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldDeleteExistingChannel)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel channel{id, name, user, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();
    }

    const auto domainChannel = domain::Channel{id, name, userMapper->mapToDomainUser(user), createdAt, updatedAt};

    channelRepository->deleteChannel({domainChannel});

    typedef odb::query<Channel> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Channel> foundChannel(db->query_one<Channel>(query::id == id));

        ASSERT_FALSE(foundChannel);

        transaction.commit();
    }
}

TEST_F(ChannelRepositoryIntegrationTest, delete_givenNonExistingChannel_shouldThrowError)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainChannel = domain::Channel{id, name, userMapper->mapToDomainUser(user), createdAt, updatedAt};

    ASSERT_ANY_THROW(channelRepository->deleteChannel({domainChannel}));
}

TEST_F(ChannelRepositoryIntegrationTest, shouldFindExistingChannelById)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel channel{id, name, user, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();
    }

    const auto foundChannel = channelRepository->findChannelById({id});

    ASSERT_TRUE(foundChannel);
    ASSERT_EQ(foundChannel->get()->getId(), id);
}

TEST_F(ChannelRepositoryIntegrationTest, givenNonExistingChannel_shouldNotFindAnyChannelById)
{
    const auto id = "id";

    const auto channel = channelRepository->findChannelById({id});

    ASSERT_FALSE(channel);
}
