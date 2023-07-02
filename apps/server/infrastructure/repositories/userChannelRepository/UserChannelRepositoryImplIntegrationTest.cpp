#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
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

    std::shared_ptr<User> createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto user = std::make_shared<User>(id, email, password, email, currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::shared_ptr<Channel> createChannel(const std::string& id, const std::string& name, const std::string& creatorId)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto channel = std::make_shared<Channel>(id, name, creatorId, currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(channel);

        transaction.commit();

        return channel;
    }

    UserChannel createUserChannel(const std::string& id, std::shared_ptr<User> user, std::shared_ptr<Channel> channel)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        UserChannel userChannel{id, std::move(user), std::move(channel), currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(userChannel);

        transaction.commit();

        return userChannel;
    }

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>();

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::UserChannelRepository> userChannelRepository =
        std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(db, userChannelMapper, userMapper,
                                                                            channelMapper);
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
    const auto creatorId = user->getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = userChannelRepository->createUserChannel(
        {id, userMapper->mapToDomainUser(user), channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(userChannel.getUser()->getId(), userId);
    ASSERT_EQ(userChannel.getChannel()->getId(), channelId);
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
    const auto creatorId = user->getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, user, channel);

    const auto domainUserChannel = domain::UserChannel{userChannel.getId(), userMapper->mapToDomainUser(user),
                                                       channelMapper->mapToDomainChannel(channel),
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

TEST_F(UserChannelRepositoryIntegrationTest, delete_givenNonExistingUserChannel_shouldThrowError)
{
    const auto id = "id1";
    const auto userId = "userId";
    const auto channelId = "channelId";
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    const auto userEmail = "email@gmail.com";
    const auto userPassword = "password";

    const auto user = createUser(userId, userEmail, userPassword);

    const auto name = "name";
    const auto creatorId = user->getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto domainUserChannel = domain::UserChannel{
        id, userMapper->mapToDomainUser(user), channelMapper->mapToDomainChannel(channel), createdAt, updatedAt};

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
    const auto creatorId = user->getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, user, channel);

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
    const auto creatorId = user->getId();

    const auto channel = createChannel(channelId, name, creatorId);

    const auto userChannel = createUserChannel(id, user, channel);

    const auto foundUsersChannels = userChannelRepository->findUsersChannelsByChannelId({channelId});

    ASSERT_TRUE(foundUsersChannels.size());
    ASSERT_EQ(foundUsersChannels[0].getId(), userChannel.getId());
}
