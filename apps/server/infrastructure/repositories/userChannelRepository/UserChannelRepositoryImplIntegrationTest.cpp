#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "User.h"
#include "User.odb.h"
#include "UserChannel.h"
#include "UserChannel.odb.h"
#include "UserChannelRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserChannelRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users_channels\";");

        db->execute("DELETE FROM \"channels\";");

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users_channels\";");

        db->execute("DELETE FROM \"channels\";");

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    User createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        User user{id, email, password, email, currentDate, currentDate};

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

    UserChannel createUserChannel(const std::string& id, const std::string& userId, const std::string& channelId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        UserChannel userChannel{id, userId, channelId, currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(userChannel);

        transaction.commit();

        return userChannel;
    }

    std::unique_ptr<server::infrastructure::UserChannelMapper> userChannelMapperInit =
        std::make_unique<server::infrastructure::UserChannelMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::UserChannelRepository> userChannelRepository =
        std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(db, std::move(userChannelMapperInit));
};

TEST_F(UserChannelRepositoryIntegrationTest, shouldCreateUserChannel)
{
    const auto id = "id1";

    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = userChannelRepository->createUserChannel({id, userId, channelId});

    ASSERT_EQ(userChannel.getUserId(), userId);
    ASSERT_EQ(userChannel.getChannelId(), channelId);
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldDeleteExistingUserChannel)
{
    const auto id = "id1";

    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, userId, channelId);

    const auto domainUserChannel =
        domain::UserChannel{userChannel.getId(), userChannel.getUserId(), userChannel.getChannelId(),
                            userChannel.getCreatedAt(), userChannel.getUpdatedAt()};

    userChannelRepository->deleteUserChannel({domainUserChannel});

    typedef odb::query<UserChannel> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<UserChannel> foundUserChannel(db->query_one<UserChannel>(query::id == id));

        ASSERT_FALSE(foundUserChannel);

        transaction.commit();
    }
}

TEST_F(UserChannelRepositoryIntegrationTest, delete_givenNonExistingUser_shouldThrowError)
{
    const auto id = "id1";
    const auto userId = "userId";
    const auto channelId = "channelId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto domainUserChannel = domain::UserChannel{id, userId, channelId, createdAt, updatedAt};

    ASSERT_ANY_THROW(userChannelRepository->deleteUserChannel({domainUserChannel}));
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldFindUsersChannelsByUserId)
{
    const auto id = "id1";

    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, userId, channelId);

    const auto foundUsersChannels = userChannelRepository->findUsersChannelsByUserId({userId});

    ASSERT_TRUE(foundUsersChannels.size());
    ASSERT_EQ(foundUsersChannels[0].getId(), userChannel.getId());
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldFindUsersChannelsByChannelId)
{
    const auto id = "id1";

    const auto userId = "userId";
    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto channelId = "channelId";
    const auto name = "name";
    const auto creatorId = user.getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, userId, channelId);

    const auto foundUsersChannels = userChannelRepository->findUsersChannelsByChannelId({channelId});

    ASSERT_TRUE(foundUsersChannels.size());
    ASSERT_EQ(foundUsersChannels[0].getId(), userChannel.getId());
}
