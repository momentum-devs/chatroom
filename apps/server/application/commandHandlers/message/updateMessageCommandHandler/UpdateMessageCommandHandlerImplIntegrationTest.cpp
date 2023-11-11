#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
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
#include "UpdateMessageCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

class UpdateMessageCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userTestUtils.truncateTable();

        messageTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();

        messageTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    MessageTestUtils messageTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<MessageRepository> messageRepository =
        std::make_shared<MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper, groupMapper);

    UpdateMessageCommandHandlerImpl updateMessageCommandHandler{messageRepository};
};

TEST_F(UpdateMessageCommandImplIntegrationTest, updateNotExistingMessage_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto userId = faker::String::uuid();
    const auto content = faker::Word::noun();

    ASSERT_THROW(updateMessageCommandHandler.execute({id, content, userId}), errors::ResourceNotFoundError);
}

TEST_F(UpdateMessageCommandImplIntegrationTest, requesterUserIsMessageSender_shouldUpdateContent)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto content = faker::Word::noun();

    const auto [updatedMessage] = updateMessageCommandHandler.execute({message->getId(), content, user->getId()});

    ASSERT_EQ(updatedMessage.getId(), message->getId());
    ASSERT_EQ(updatedMessage.getContent(), content);
}

TEST_F(UpdateMessageCommandImplIntegrationTest, requesterUserIsNotMessageSender_shouldThrow)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto content = faker::Word::noun();

    ASSERT_THROW(updateMessageCommandHandler.execute({message->getId(), content, id}), errors::OperationNotValidError);
}
