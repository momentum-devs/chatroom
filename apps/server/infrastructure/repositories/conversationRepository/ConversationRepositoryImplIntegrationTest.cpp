#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "ConversationRepositoryImpl.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/conversationTestUtils/ConversationTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ConversationRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        conversationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        conversationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    ConversationTestUtils conversationTestUtils{db};

    UserTestFactory userTestFactory;

    ConversationTestFactory conversationTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ConversationMapper> conversationMapper =
        std::make_shared<ConversationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<server::domain::ConversationRepository> conversationRepository =
        std::make_shared<server::infrastructure::ConversationRepositoryImpl>(db, conversationMapper, userMapper,
                                                                             channelMapper);
};

TEST_F(ConversationRepositoryIntegrationTest, shouldCreateUsersConversation)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto conversation = conversationRepository->createConversation(
        {id, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient), std::nullopt});

    ASSERT_EQ(conversation.getUser()->getId(), user->getId());
    ASSERT_EQ(conversation.getRecipient()->getId(), recipient->getId());
    ASSERT_EQ(conversation.getChannel(), nullptr);
}

TEST_F(ConversationRepositoryIntegrationTest, shouldCreateChannelConversation)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto conversation = conversationRepository->createConversation(
        {id, std::nullopt, std::nullopt, channelMapper->mapToDomainChannel(channel)});

    ASSERT_EQ(conversation.getChannel()->getId(), channel->getId());
    ASSERT_EQ(conversation.getUser(), nullptr);
    ASSERT_EQ(conversation.getRecipient(), nullptr);
}

TEST_F(ConversationRepositoryIntegrationTest, shouldDeleteExistingConversation)
{
    const auto user = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto conversation = conversationTestUtils.createAndPersist(user, recipient);

    const auto domainConversation = conversationMapper->mapToDomainConversation(*conversation);

    conversationRepository->deleteConversation({domainConversation});

    const auto foundConversation = conversationTestUtils.findById(conversation->getId());

    ASSERT_FALSE(foundConversation);
}

TEST_F(ConversationRepositoryIntegrationTest, delete_givenNonExistingConversation_shouldThrowError)
{
    const auto user = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto domainConversation = conversationTestFactory.createDomainUsersConversation(
        userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(recipient));

    ASSERT_ANY_THROW(conversationRepository->deleteConversation({*domainConversation}));
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationByUserIds)
{
    const auto user = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto conversation = conversationTestUtils.createAndPersist(user, recipient);

    const auto foundConversation = conversationRepository->findConversationByUserAndRecipientIds({user->getId(), recipient->getId()});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation->getId());
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationByChannelId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto conversation = conversationTestUtils.createAndPersist(channel);

    const auto foundConversation = conversationRepository->findConversationByChannelId({channel->getId()});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation->getId());
    ASSERT_EQ(foundConversation->getChannel()->getId(), channel->getId());
}

TEST_F(ConversationRepositoryIntegrationTest, shouldFindConversationById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto conversation = conversationTestUtils.createAndPersist(user, recipient);

    const auto foundConversation = conversationRepository->findConversationById({conversation->getId()});

    ASSERT_TRUE(foundConversation);
    ASSERT_EQ(foundConversation->getId(), conversation->getId());
}
