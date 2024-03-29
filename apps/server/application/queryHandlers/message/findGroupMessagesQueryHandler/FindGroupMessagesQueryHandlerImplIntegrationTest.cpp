#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "FindGroupMessagesQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/messageTestUtils/MessageTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

class FindGroupMessagesQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        messageTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        messageTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    MessageTestUtils messageTestUtils{db};
    GroupTestUtils groupTestUtils{db};
    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<MessageMapper> messageMapper =
        std::make_shared<MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    std::shared_ptr<MessageRepository> messageRepository =
        std::make_shared<MessageRepositoryImpl>(db, messageMapper, userMapper, channelMapper, groupMapper);

    FindGroupMessagesQueryHandlerImpl findGroupMessagesQueryHandler{messageRepository};
};

TEST_F(FindGroupMessagesQueryHandlerImplIntegrationTest, findMessagesByChannel)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto message = messageTestUtils.createAndPersist(user, group);

    const auto [messages, count] = findGroupMessagesQueryHandler.execute({group->getId()});

    ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0].getId(), message->getId());
    ASSERT_EQ(count, 1);
}
