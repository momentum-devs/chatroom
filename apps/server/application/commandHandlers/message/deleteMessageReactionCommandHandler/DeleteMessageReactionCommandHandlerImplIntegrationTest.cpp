#include "gtest/gtest.h"

#include "DeleteMessageReactionCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapper.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapperImpl.h"
#include "server/infrastructure/repositories/reactionRepository/ReactionRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/factories/reactionTestFactory/ReactionTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/reactionTestUtils/ReactionTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class DeleteMessageReactionCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        reactionTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        reactionTestUtils.truncateTable();

        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    ReactionTestUtils reactionTestUtils{db};
    MessageTestUtils messageTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<ReactionMapper> reactionMapper = std::make_shared<ReactionMapperImpl>(userMapper, messageMapper);

    std::shared_ptr<domain::ReactionRepository> reactionRepository =
        std::make_shared<infrastructure::ReactionRepositoryImpl>(db, reactionMapper, userMapper, messageMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<server::domain::MessageRepository> messageRepository =
        std::make_shared<server::infrastructure::MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper,
                                                                        groupMapper);

    DeleteMessageReactionCommandHandlerImpl deleteMessageReactionCommandHandler{userRepository, messageRepository,
                                                                                reactionRepository};
};

TEST_F(DeleteMessageReactionCommandImplIntegrationTest, deleteMessageReaction)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    deleteMessageReactionCommandHandler.execute({reaction->getId(), user->getId(), message->getId()});

    const auto deletedMessage = reactionRepository->findReactionById({reaction->getId()});

    ASSERT_FALSE(deletedMessage);
}

TEST_F(DeleteMessageReactionCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto userId = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reactionName = faker::Word::noun();

    ASSERT_THROW(deleteMessageReactionCommandHandler.execute({reactionName, userId, message->getId()}),
                 errors::ResourceNotFoundError);
}

TEST_F(DeleteMessageReactionCommandImplIntegrationTest, givenNotExistingMessage_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto messageId = faker::String::uuid();

    const auto reactionName = faker::Word::noun();

    ASSERT_THROW(deleteMessageReactionCommandHandler.execute({reactionName, user->getId(), messageId}),
                 errors::ResourceNotFoundError);
}

TEST_F(DeleteMessageReactionCommandImplIntegrationTest, givenNotExistingReaction_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    const auto messageId = faker::String::uuid();

    const auto reactionName = faker::Word::noun();

    ASSERT_THROW(deleteMessageReactionCommandHandler.execute({reactionName, user->getId(), messageId}),
                 errors::ResourceNotFoundError);
}
