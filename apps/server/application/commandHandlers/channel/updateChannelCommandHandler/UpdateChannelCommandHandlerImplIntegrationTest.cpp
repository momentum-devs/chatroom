#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "UpdateChannelCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

class UpdateChannelCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userTestUtils.truncateTable();

        channelTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();

        channelTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    ChannelTestUtils channelTestUtils{db};
    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    UpdateChannelCommandHandlerImpl updateChannelCommandHandler{channelRepository};
};

TEST_F(UpdateChannelCommandImplIntegrationTest, updateNotExistingChannel_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto name = faker::Word::noun();

    ASSERT_THROW(updateChannelCommandHandler.execute({id, name, userId}), errors::ResourceNotFoundError);
}

TEST_F(UpdateChannelCommandImplIntegrationTest, requesterUserIsChannelCreator_shouldUpdateName)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto name = faker::Word::noun();

    const auto [updatedChannel] = updateChannelCommandHandler.execute({channel->getId(), name, user->getId()});

    ASSERT_EQ(updatedChannel.getId(), channel->getId());
    ASSERT_EQ(updatedChannel.getName(), name);
}

TEST_F(UpdateChannelCommandImplIntegrationTest, requesterUserIsNotChannelCreator_shouldThrow)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto name = faker::Word::noun();

    ASSERT_THROW(updateChannelCommandHandler.execute({channel->getId(), name, id}), errors::OperationNotValidError);
}
