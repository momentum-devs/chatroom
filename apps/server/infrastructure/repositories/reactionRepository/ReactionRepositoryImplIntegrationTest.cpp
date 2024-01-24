#include <odb/sqlite/database.hxx>

#include "gtest/gtest.h"

#include "../../../tests/factories/userTestFactory/UserTestFactory.h"
#include "faker-cxx/String.h"
#include "faker-cxx/Word.h"
#include "ReactionRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/reactionTestUtils/ReactionTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ReactionRepositoryIntegrationTest : public Test
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

    UserTestFactory userTestFactory;
    ChannelTestFactory channelTestFactory;

    ReactionTestFactory reactionTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<ReactionMapper> reactionMapper = std::make_shared<ReactionMapperImpl>(userMapper, messageMapper);

    std::shared_ptr<server::domain::ReactionRepository> reactionRepository =
        std::make_shared<server::infrastructure::ReactionRepositoryImpl>(db, reactionMapper, userMapper, messageMapper);
};

TEST_F(ReactionRepositoryIntegrationTest, shouldCreateReaction)
{
    const auto id = faker::String::uuid();

    const auto name = faker::Word::noun();

    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionRepository->createReaction(
        {id, name, userMapper->mapToDomainUser(user), messageMapper->mapToDomainMessage(message)});

    const auto foundReaction = reactionTestUtils.findById(reaction.getId());

    ASSERT_TRUE(foundReaction);
    ASSERT_EQ(reaction.getUser()->getId(), user->getId());
    ASSERT_EQ(reaction.getMessage()->getId(), message->getId());
}

TEST_F(ReactionRepositoryIntegrationTest, shouldDeleteExistingReaction)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    const auto domainReaction = reactionMapper->mapToDomainReaction(*reaction);

    reactionRepository->deleteReaction({domainReaction});

    const auto foundReaction = reactionTestUtils.findById(reaction->getId());

    ASSERT_FALSE(foundReaction);
}

TEST_F(ReactionRepositoryIntegrationTest, delete_givenNonExistingReaction_shouldThrowError)
{
    const auto user = userTestUtils.createAndPersist();

    const auto domainUser = userMapper->mapToDomainUser(user);

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto domainChannel = channelMapper->mapToDomainChannel(channel);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto domainMessage = messageMapper->mapToDomainMessage(message);

    const auto domainReaction = reactionTestFactory.createDomainReaction(domainUser, domainMessage);

    ASSERT_ANY_THROW(reactionRepository->deleteReaction({*domainReaction}));
}

TEST_F(ReactionRepositoryIntegrationTest, shouldFindReactionsByMessageId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    const auto foundReactions = reactionRepository->findReactionsByMessageId({message->getId()});

    ASSERT_TRUE(foundReactions.size());
    ASSERT_EQ(foundReactions[0].getId(), reaction->getId());
}

TEST_F(ReactionRepositoryIntegrationTest, shouldFindReactionById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    const auto foundReaction = reactionRepository->findReactionById({reaction->getId()});

    ASSERT_TRUE(foundReaction);
    ASSERT_EQ(foundReaction->getId(), reaction->getId());
}

TEST_F(ReactionRepositoryIntegrationTest, shouldFindReactionByUserIdAndMessageId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    const auto foundReaction = reactionRepository->findReaction({user->getId(), message->getId()});

    ASSERT_TRUE(foundReaction);
    ASSERT_EQ(foundReaction->getId(), reaction->getId());
}
