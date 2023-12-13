#include "gtest/gtest.h"

#include "FindMessageReactionsQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapper.h"
#include "server/infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapperImpl.h"
#include "server/infrastructure/repositories/reactionRepository/ReactionRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "server/tests/utils/reactionTestUtils/ReactionTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

class FindMessageReactionsQueryHandlerImplIntegrationTest : public Test
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

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    MessageTestUtils messageTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    UserTestUtils userTestUtils{db};
    ReactionTestUtils reactionTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<ReactionMapper> reactionMapper = std::make_shared<ReactionMapperImpl>(userMapper, messageMapper);

    std::shared_ptr<domain::ReactionRepository> reactionRepository =
        std::make_shared<infrastructure::ReactionRepositoryImpl>(db, reactionMapper, userMapper, messageMapper);

    FindMessageReactionsQueryHandlerImpl findMessageReactionsQueryHandler{reactionRepository};
};

TEST_F(FindMessageReactionsQueryHandlerImplIntegrationTest, findReactionsByMessageId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, channel);

    const auto reaction = reactionTestUtils.createAndPersist(user, message);

    const auto [reactions] = findMessageReactionsQueryHandler.execute({message->getId()});

    ASSERT_EQ(reactions.size(), 1);
    ASSERT_EQ(reactions[0].getId(), reaction->getId());
}
