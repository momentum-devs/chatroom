#include "gtest/gtest.h"

#include "DeleteMessageCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class DeleteMessageCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    MessageTestUtils messageTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper = std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<domain::MessageRepository> messageRepository =
        std::make_shared<MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper, groupMapper);

    DeleteMessageCommandHandlerImpl deleteMessageCommandHandler{messageRepository};
};

TEST_F(DeleteMessageCommandImplIntegrationTest, givenExistingMessageAndRequesterIsMessageSender_shouldDeleteMessage)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(sender, channel);

    deleteMessageCommandHandler.execute({message->getId(), sender->getId()});

    const auto foundMessage = messageTestUtils.findById(message->getId());

    ASSERT_FALSE(foundMessage);
}

TEST_F(DeleteMessageCommandImplIntegrationTest, givenExistingMessageAndRequesterIsNotMessageSender_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(sender, channel);

    ASSERT_THROW(deleteMessageCommandHandler.execute({message->getId(), user->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(DeleteMessageCommandImplIntegrationTest, givenNonExistingMessage_shouldThrow)
{
    const auto userId = faker::String::uuid();

    const auto messageId = faker::String::uuid();

    ASSERT_THROW(deleteMessageCommandHandler.execute({messageId, userId}), errors::ResourceNotFoundError);
}
