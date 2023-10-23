#include "gtest/gtest.h"

#include "DeleteChannelCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class DeleteChannelCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    DeleteChannelCommandHandlerImpl deleteChannelCommandHandler{channelRepository};
};

TEST_F(DeleteChannelCommandImplIntegrationTest, givenExistingChannelAndRequesterIsChannelCreator_shouldDeleteChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    deleteChannelCommandHandler.execute({channel->getId(), user->getId()});

    const auto foundChannel = channelTestUtils.findById(channel->getId());

    ASSERT_FALSE(foundChannel);
}

TEST_F(DeleteChannelCommandImplIntegrationTest, givenExistingChannelAndRequesterIsNotChannelCreator_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channel->getId(), user->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(DeleteChannelCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto userId = faker::String::uuid();

    const auto channelId = faker::String::uuid();

    ASSERT_THROW(deleteChannelCommandHandler.execute({channelId, userId}), errors::ResourceNotFoundError);
}
