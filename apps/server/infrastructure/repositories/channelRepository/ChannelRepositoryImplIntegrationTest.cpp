#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.odb.h"
#include "ChannelRepositoryImpl.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
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

    User createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        User user{id, email, password, email, false, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::unique_ptr<server::infrastructure::ChannelMapper> channelMapperInit =
        std::make_unique<server::infrastructure::ChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::ChannelRepository> channelRepository =
        std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, std::move(channelMapperInit));
};

TEST_F(ChannelRepositoryIntegrationTest, shouldCreateChannel)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = channelRepository->createChannel({id, name, creatorId});

    ASSERT_EQ(channel->getId(), id);
    ASSERT_EQ(channel->getName(), name);
    ASSERT_EQ(channel->getCreatorId(), creatorId);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldDeleteExistingChannel)
{
    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto id = "id1";
    const auto name = "name";
    const auto creatorId = user.getId();
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel channel{id, name, creatorId, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();
    }

    const auto domainChannel = domain::Channel{id, name, creatorId, createdAt, updatedAt};

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
    const auto id = "id1";
    const auto name = "name";
    const auto creatorId = "creatorId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainChannel = domain::Channel{id, name, creatorId, createdAt, updatedAt};

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
    const auto creatorId = user.getId();
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel channel{id, name, creatorId, createdAt, updatedAt};

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
