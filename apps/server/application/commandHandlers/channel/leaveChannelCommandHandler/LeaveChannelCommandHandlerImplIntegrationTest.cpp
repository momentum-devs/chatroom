#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "LeaveChannelCommandHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class LeaveChannelCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<domain::UserChannelRepository> userChannelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    LeaveChannelCommandHandlerImpl leaveChannelCommandHandler{userChannelRepository, channelRepository};
};

TEST_F(LeaveChannelCommandImplIntegrationTest, givenUserWhoIsNotChannelCreator_shouldDeleteUserChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    leaveChannelCommandHandler.execute({user->getId(), channel->getId()});

    const auto foundUserChannel = userChannelTestUtils.find(user->getId(), channel->getId());

    ASSERT_FALSE(foundUserChannel);
}

TEST_F(LeaveChannelCommandImplIntegrationTest, givenUserWhoIsChannelCreator_shouldDeleteUserChannelAndChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    leaveChannelCommandHandler.execute({user->getId(), channel->getId()});

    const auto foundUserChannel = userChannelTestUtils.find(user->getId(), channel->getId());

    ASSERT_FALSE(foundUserChannel);

    const auto foundChannel = channelTestUtils.findById(channel->getId());

    ASSERT_FALSE(foundChannel);
}

TEST_F(LeaveChannelCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto userId = faker::String::uuid();

    const auto channelId = faker::String::uuid();

    ASSERT_THROW(leaveChannelCommandHandler.execute({userId, channelId}), errors::ResourceNotFoundError);
}

TEST_F(LeaveChannelCommandImplIntegrationTest, givenUserNotMemberOfChannel_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    ASSERT_THROW(leaveChannelCommandHandler.execute({user->getId(), channel->getId()}), errors::ResourceNotFoundError);
}
