#include "gtest/gtest.h"

#include "FindChannelConversationQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapper.h"
#include "server/infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImpl.h"
#include "server/infrastructure/repositories/conversationRepository/ConversationRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/conversationTestUtils/ConversationTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindChannelConversationQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();

        conversationTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();

        conversationTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    ConversationTestUtils conversationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<ConversationMapper> conversationMapper =
        std::make_shared<ConversationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::ConversationRepository> conversationRepository =
        std::make_shared<ConversationRepositoryImpl>(db, conversationMapper, userMapper, channelMapper);

    FindChannelConversationQueryHandlerImpl findChannelConversationQueryHandler{conversationRepository};
};

TEST_F(FindChannelConversationQueryHandlerImplIntegrationTest, findChannelsById)
{
    const auto channel = channelTestUtils.createAndPersist();

    const auto conversation = conversationTestUtils.createAndPersist(channel);

    const auto [foundConversation] = findChannelConversationQueryHandler.execute({channel->getId()});

    ASSERT_EQ(foundConversation.getId(), conversation->getId());
    ASSERT_EQ(foundConversation.getChannel()->getId(), channel->getId());
}
