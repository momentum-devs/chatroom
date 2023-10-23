#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "RemoveUserFromChannelCommandHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class RemoveUserFromChannelCommandImplIntegrationTest : public Test
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

    std::shared_ptr<domain::UserChannelRepository> channelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    RemoveUserFromChannelCommandHandlerImpl removeUserFromChannelCommandHandler{channelRepository};
};

TEST_F(RemoveUserFromChannelCommandImplIntegrationTest, givenExistingUserChannel_shouldDeleteUserChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    removeUserFromChannelCommandHandler.execute({user->getId(), channel->getId()});

    const auto foundUserChannel = userChannelTestUtils.find(user->getId(), channel->getId());

    ASSERT_FALSE(foundUserChannel);
}

TEST_F(RemoveUserFromChannelCommandImplIntegrationTest, givenNonExistingUserChannel_shouldThrow)
{
    const auto userId = faker::String::uuid();

    const auto userChannelId = faker::String::uuid();

    ASSERT_THROW(removeUserFromChannelCommandHandler.execute({userId, userChannelId}), errors::ResourceNotFoundError);
}
