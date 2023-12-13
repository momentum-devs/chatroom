#include "gtest/gtest.h"

#include "FindUsersBelongingToChannelQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindUsersBelongingToChannelQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    UserChannelTestUtils userChannelTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<UserChannelMapper> userChannelMapper =
        std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::UserChannelRepository> channelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    FindUsersBelongingToChannelQueryHandlerImpl findUsersBelongingToChannelQueryHandler{channelRepository};
};

TEST_F(FindUsersBelongingToChannelQueryHandlerImplIntegrationTest, findUsersByChannelId)
{
    const auto user1 = userTestUtils.createAndPersist();

    const auto user2 = userTestUtils.createAndPersist();

    const auto channel1 = channelTestUtils.createAndPersist(user1);

    const auto channel2 = channelTestUtils.createAndPersist(user1);

    userChannelTestUtils.createAndPersist(user1, channel1);

    userChannelTestUtils.createAndPersist(user2, channel2);

    const auto [users] = findUsersBelongingToChannelQueryHandler.execute({channel2->getId()});

    ASSERT_EQ(users.size(), 1);
    ASSERT_EQ(users[0].getId(), user2->getId());
}
