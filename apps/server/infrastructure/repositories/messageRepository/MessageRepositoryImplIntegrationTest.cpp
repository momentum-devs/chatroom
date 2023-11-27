#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class MessageRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();

        groupTestUtils.truncateTable();
    }

    void TearDown() override
    {
        messageTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();

        groupTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    MessageTestUtils messageTestUtils{db};
    GroupTestUtils groupTestUtils{db};

    UserTestFactory userTestFactory;
    MessageTestFactory messageTestFactory;
    GroupTestFactory groupTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<server::domain::MessageRepository> messageRepository =
        std::make_shared<server::infrastructure::MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper,
                                                                        groupMapper);
};

TEST_F(MessageRepositoryIntegrationTest, shouldCreateChannelMessage)
{
    const auto id = faker::String::uuid();

    const auto content = faker::Word::words();

    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto message = messageRepository->createMessage(
        {id, content, userMapper->mapToDomainUser(sender), channelMapper->mapToDomainChannel(channel), std::nullopt});

    ASSERT_EQ(message->getSender()->getId(), sender->getId());
    ASSERT_EQ(message->getChannel()->getId(), channel->getId());
    ASSERT_EQ(message->getGroup(), nullptr);

    const auto foundMessage = messageTestUtils.findById(id);

    ASSERT_TRUE(foundMessage);
}

TEST_F(MessageRepositoryIntegrationTest, shouldCreateGroupMessage)
{
    const auto id = faker::String::uuid();

    const auto content = faker::Word::words();

    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageRepository->createMessage(
        {id, content, userMapper->mapToDomainUser(sender), std::nullopt, groupMapper->mapToDomainGroup(group)});

    ASSERT_EQ(message->getSender()->getId(), sender->getId());
    ASSERT_EQ(message->getChannel(), nullptr);
    ASSERT_EQ(message->getGroup()->getId(), group->getId());

    const auto foundMessage = messageTestUtils.findById(id);

    ASSERT_TRUE(foundMessage);
}

TEST_F(MessageRepositoryIntegrationTest, shouldDeleteExistingMessage)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto message = messageTestUtils.createAndPersist(sender, channel);

    const auto domainMessage = messageMapper->mapToDomainMessage(message);

    messageRepository->deleteMessage({*domainMessage});

    const auto foundMessage = messageTestUtils.findById(message->getId());

    ASSERT_FALSE(foundMessage);
}

TEST_F(MessageRepositoryIntegrationTest, delete_givenNonExistingMessage_shouldThrowError)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto domainMessage = messageTestFactory.createDomainMessage(
        userMapper->mapToDomainUser(sender), channelMapper->mapToDomainChannel(channel), nullptr);

    ASSERT_ANY_THROW(messageRepository->deleteMessage({*domainMessage}));
}

TEST_F(MessageRepositoryIntegrationTest, shouldFindMessageById)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto message = messageTestUtils.createAndPersist(sender, channel);

    const auto foundMessage = messageRepository->findMessageById({message->getId()});

    ASSERT_TRUE(foundMessage);
    ASSERT_EQ(foundMessage->get()->getId(), message->getId());
}

TEST_F(MessageRepositoryIntegrationTest, shouldFindMessagesByChannelId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto message = messageTestUtils.createAndPersist(sender, channel);

    const auto foundMessages = messageRepository->findMessagesByChannelId({channel->getId()});

    ASSERT_EQ(foundMessages.size(), 1);
    ASSERT_EQ(foundMessages[0]->getId(), message->getId());
    ASSERT_EQ(foundMessages[0]->getChannel()->getId(), channel->getId());
}

TEST_F(MessageRepositoryIntegrationTest, shouldFindMessagesByChannelIdWithPagination)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);

    const auto foundMessages = messageRepository->findMessagesByChannelId({channel->getId(), 0, 4});

    ASSERT_EQ(foundMessages.size(), 4);
}

TEST_F(MessageRepositoryIntegrationTest, shouldCountMessagesByChannelId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);
    messageTestUtils.createAndPersist(sender, channel);

    const auto count = messageRepository->countMessagesByChannelId({channel->getId()});

    ASSERT_EQ(count, 6);
}

TEST_F(MessageRepositoryIntegrationTest, shouldFindMessagesByGroupId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(sender, group);

    const auto foundMessages = messageRepository->findMessagesByGroupId({group->getId()});

    ASSERT_EQ(foundMessages.size(), 1);
    ASSERT_EQ(foundMessages[0]->getId(), message->getId());
    ASSERT_EQ(foundMessages[0]->getGroup()->getId(), group->getId());
}

TEST_F(MessageRepositoryIntegrationTest, shouldFindMessagesByGroupIdWithPagination)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);

    const auto foundMessages = messageRepository->findMessagesByGroupId({group->getId(), 0, 3});

    ASSERT_EQ(foundMessages.size(), 3);
}

TEST_F(MessageRepositoryIntegrationTest, shouldCountMessagesByGroupId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);
    messageTestUtils.createAndPersist(sender, group);

    const auto count = messageRepository->countMessagesByGroupId({group->getId()});

    ASSERT_EQ(count, 4);
}

TEST_F(MessageRepositoryIntegrationTest, shouldUpdateExistingMessage)
{
    const auto updatedContent = faker::Word::words();

    const auto sender = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(sender, group);

    const auto domainMessage = messageMapper->mapToDomainMessage(message);

    domainMessage->setContent(updatedContent);

    messageRepository->updateMessage({*domainMessage});

    const auto updatedMessage = messageTestUtils.findById({message->getId()});

    ASSERT_TRUE(updatedMessage);
    ASSERT_EQ((*updatedMessage).getContent(), updatedContent);
}

TEST_F(MessageRepositoryIntegrationTest, update_givenNonExistingMessage_shouldThrowError)
{
    const auto sender = userTestFactory.createDomainUser();

    const auto group = groupTestFactory.createDomainGroup();

    const auto domainMessage = messageTestFactory.createDomainMessage(sender, nullptr, group);

    ASSERT_ANY_THROW(messageRepository->updateMessage({*domainMessage}));
}
