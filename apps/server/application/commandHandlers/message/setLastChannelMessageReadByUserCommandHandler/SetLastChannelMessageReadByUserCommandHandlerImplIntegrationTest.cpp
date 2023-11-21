#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapper.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "SetLastChannelMessageReadByUserCommandHandlerImpl.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class SetLastChannelMessageReadByUserCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userChannelTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userChannelTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    ChannelTestFactory channelTestFactory;

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    MessageTestUtils messageTestUtils{db};
    UserChannelTestUtils userChannelTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::UserChannelRepository> userChannelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<server::domain::MessageRepository> messageRepository =
        std::make_shared<server::infrastructure::MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper,
                                                                        groupMapper);

    SetLastChannelMessageReadByUserCommandHandlerImpl setLastChannelMessageReadByUserCommandHandler{
        channelRepository, userRepository, userChannelRepository, messageRepository};
};

TEST_F(SetLastChannelMessageReadByUserCommandImplIntegrationTest, setLastChannelMessageReadByUser)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto userChannel = userChannelTestUtils.createAndPersist(user, channel);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    setLastChannelMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), channel->getId()});

    const auto updatedUserChannel = userChannelTestUtils.findById(userChannel->getId());

    ASSERT_EQ(updatedUserChannel->getLastReadMessageId().get(), message->getId());
}

TEST_F(SetLastChannelMessageReadByUserCommandImplIntegrationTest, givenNotExistingMessage_shouldThrow)
{
    const auto messageId = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    userChannelTestUtils.createAndPersist(user, channel);

    ASSERT_THROW(setLastChannelMessageReadByUserCommandHandler.execute({messageId, user->getId(), channel->getId()}),
                 errors::ResourceNotFoundError);
}

TEST_F(SetLastChannelMessageReadByUserCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto nonExistingUserId = faker::String::uuid();

    ASSERT_THROW(
        setLastChannelMessageReadByUserCommandHandler.execute({message->getId(), nonExistingUserId, channel->getId()}),
        errors::ResourceNotFoundError);
}

TEST_F(SetLastChannelMessageReadByUserCommandImplIntegrationTest, givenNotExistingChannel_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto nonExistingChannelId = faker::String::uuid();

    ASSERT_THROW(
        setLastChannelMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), nonExistingChannelId}),
        errors::ResourceNotFoundError);
}

TEST_F(SetLastChannelMessageReadByUserCommandImplIntegrationTest, givenUserWhichIsNotMemberOfChannel_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    ASSERT_THROW(
        setLastChannelMessageReadByUserCommandHandler.execute({message->getId(), user->getId(), channel->getId()}),
        errors::ResourceNotFoundError);
}
