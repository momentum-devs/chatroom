#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.odb.h"
#include "ChannelRepositoryImpl.h"
#include "server/infrastructure/errors/ChannelRepositoryError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/ChannelMapperImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class ChannelRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    std::unique_ptr<server::infrastructure::ChannelMapper> userMapperInit =
        std::make_unique<server::infrastructure::ChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::ChannelRepository> userRepository =
        std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, std::move(userMapperInit));
};

TEST_F(ChannelRepositoryIntegrationTest, shouldCreateChannel)
{
    const auto id = "id1";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = userRepository->createChannel({id, email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), password);
    ASSERT_EQ(user.getNickname(), nickname);
}

TEST_F(ChannelRepositoryIntegrationTest, givenChannelWithExistingEmail_shouldThrowError)
{
    const auto id1 = "id1";
    const auto id2 = "id2";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel user{id1, email, password, nickname, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    ASSERT_THROW(userRepository->createChannel({id2, email, password, nickname}), errors::ChannelRepositoryError);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldDeleteExistingChannel)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel user{id, email, password, nickname, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto domainChannel = domain::Channel{id, email, password, nickname, createdAt, updatedAt};

    userRepository->deleteChannel({domainChannel});

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
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainChannel = domain::Channel{id, email, password, nickname, createdAt, updatedAt};

    ASSERT_ANY_THROW(userRepository->deleteChannel({domainChannel}));
}

TEST_F(ChannelRepositoryIntegrationTest, shouldFindExistingChannelByEmail)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel user{id, email, password, nickname, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundChannel = userRepository->findChannelByEmail({email});

    ASSERT_TRUE(foundChannel);
    ASSERT_EQ(foundChannel->getEmail(), email);
}

TEST_F(ChannelRepositoryIntegrationTest, givenNonExistingChannel_shouldNotFindAnyChannelByEmail)
{
    const auto email = "email@example.com";

    const auto user = userRepository->findChannelByEmail({email});

    ASSERT_FALSE(user);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldFindExistingChannelById)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel user{id, email, password, nickname, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundChannel = userRepository->findChannelById({id});

    ASSERT_TRUE(foundChannel);
    ASSERT_EQ(foundChannel->getId(), id);
}

TEST_F(ChannelRepositoryIntegrationTest, givenNonExistingChannel_shouldNotFindAnyChannelById)
{
    const auto id = "id";

    const auto user = userRepository->findChannelById({id});

    ASSERT_FALSE(user);
}

TEST_F(ChannelRepositoryIntegrationTest, shouldUpdateExistingChannel)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password1";
    const auto updatedPassword = "password2";
    const auto nickname = "nickname1";
    const auto updatedNickname = "nickname2";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    Channel user{id, email, password, nickname, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    auto domainChannel = domain::Channel{id, email, password, nickname, createdAt, updatedAt};

    domainChannel.setPassword(updatedPassword);
    domainChannel.setNickname(updatedNickname);

    userRepository->updateChannel({domainChannel});

    {
        typedef odb::query<Channel> query;

        odb::transaction transaction(db->begin());

        std::shared_ptr<Channel> updatedChannel(db->query_one<Channel>(query::id == id));

        transaction.commit();

        ASSERT_TRUE(updatedChannel);
        ASSERT_EQ(updatedChannel->getNickname(), updatedNickname);
        ASSERT_EQ(updatedChannel->getPassword(), updatedPassword);
    }
}

TEST_F(ChannelRepositoryIntegrationTest, update_givenNonExistingChannel_shouldThrowError)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainChannel = domain::Channel{id, email, password, nickname, createdAt, updatedAt};

    ASSERT_ANY_THROW(userRepository->updateChannel({domainChannel}));
}
