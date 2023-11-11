#include "gtest/gtest.h"

#include "CreateChannelMessageCommandHandlerImpl.h"
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
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateChannelMessageCommandImplIntegrationTest : public Test
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

    CreateChannelMessageCommandHandlerImpl createChannelMessageCommandHandler{channelRepository, userRepository,
                                                                              userChannelRepository, messageRepository};
};

TEST_F(CreateChannelMessageCommandImplIntegrationTest, createChannelMessage)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    userChannelTestUtils.createAndPersist(sender, channel);

    const auto content = faker::Word::noun();

    const auto [createdChannel] =
        createChannelMessageCommandHandler.execute({content, sender->getId(), channel->getId()});

    ASSERT_EQ(createdChannel.getContent(), content);

    ASSERT_EQ(createdChannel.getSender()->getId(), sender->getId());

    ASSERT_EQ(createdChannel.getChannel()->getId(), channel->getId());
}

TEST_F(CreateChannelMessageCommandImplIntegrationTest, givenSenderWhichIsNotMemberOfChannel_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createChannelMessageCommandHandler.execute({content, sender->getId(), channel->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(CreateChannelMessageCommandImplIntegrationTest, givenNotExistingUser_shouldThrow)
{
    const auto senderId = faker::String::uuid();

    const auto channel = channelTestUtils.createAndPersist();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createChannelMessageCommandHandler.execute({content, senderId, channel->getId()}),
                 errors::ResourceNotFoundError);
}

TEST_F(CreateChannelMessageCommandImplIntegrationTest, givenNotExistingChannel_shouldThrow)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channelId = faker::String::uuid();

    const auto content = faker::Word::noun();

    ASSERT_THROW(createChannelMessageCommandHandler.execute({content, sender->getId(), channelId}),
                 errors::ResourceNotFoundError);
}
