#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "../../../tests/factories/userTestFactory/UserTestFactory.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "UserChannelRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserChannelRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    UserChannelTestUtils userChannelTestUtils{db};

    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;

    UserChannelTestFactory userChannelTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<server::domain::UserChannelRepository> userChannelRepository =
        std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(db, userChannelMapper, userMapper,
                                                                            channelMapper);
};

TEST_F(UserChannelRepositoryIntegrationTest, shouldCreateUserChannel)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelRepository->createUserChannel(
        {id, userMapper->mapToDomainUser(user), channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(userChannel.getUser()->getId(), user->getId());
    ASSERT_EQ(userChannel.getChannel()->getId(), channel->getId());
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldDeleteExistingUserChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    const auto domainUserChannel = userChannelMapper->mapToDomainUserChannel(*userChannel);

    userChannelRepository->deleteUserChannel({domainUserChannel});

    const auto foundUserChannel = userChannelTestUtils.findById(userChannel->getId());

    ASSERT_FALSE(foundUserChannel);
}

TEST_F(UserChannelRepositoryIntegrationTest, delete_givenNonExistingUserChannel_shouldThrowError)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto domainUser = userMapper->mapToDomainUser(user);

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto domainChannel = channelMapper->mapToDomainChannel(channel);

    const auto domainUserChannel = userChannelTestFactory.createDomainUserChannel(domainUser, domainChannel);

    ASSERT_ANY_THROW(userChannelRepository->deleteUserChannel({*domainUserChannel}));
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldFindUsersChannelsByUserId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    const auto foundUsersChannels = userChannelRepository->findUsersChannelsByUserId({user->getId()});

    ASSERT_TRUE(foundUsersChannels.size());
    ASSERT_EQ(foundUsersChannels[0].getId(), userChannel->getId());
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldFindUsersChannelsByChannelId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    const auto foundUsersChannels = userChannelRepository->findUsersChannelsByChannelId({channel->getId()});

    ASSERT_TRUE(foundUsersChannels.size());
    ASSERT_EQ(foundUsersChannels[0].getId(), userChannel->getId());
}

TEST_F(UserChannelRepositoryIntegrationTest, shouldUpdateExistingUserChannel)
{
    const auto updatedMessageId = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    auto domainUserChannel = userChannelMapper->mapToDomainUserChannel(*userChannel);

    domainUserChannel.setLastReadMessageId(updatedMessageId);

    userChannelRepository->updateUserChannel({domainUserChannel});

    const auto updatedUserChannel = userChannelTestUtils.findById({userChannel->getId()});

    ASSERT_TRUE(updatedUserChannel);
    ASSERT_EQ((*updatedUserChannel).getLastReadMessageId().get(), updatedMessageId);
}

TEST_F(UserChannelRepositoryIntegrationTest, update_givenNonExistingUserChannel_shouldThrowError)
{
    const auto user = userTestFactory.createDomainUser();

    const auto channel = channelTestFactory.createDomainChannel(user);

    const auto domainMessage = userChannelTestFactory.createDomainUserChannel(user, channel);

    ASSERT_ANY_THROW(userChannelRepository->updateUserChannel({*domainMessage}));
}
