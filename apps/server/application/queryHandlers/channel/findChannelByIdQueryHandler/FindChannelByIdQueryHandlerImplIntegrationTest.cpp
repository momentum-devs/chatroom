#include "gtest/gtest.h"

#include "FindChannelByIdQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindChannelByIdQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    FindChannelByIdQueryHandlerImpl findChannelByIdQueryHandler{channelRepository};
};

TEST_F(FindChannelByIdQueryHandlerImplIntegrationTest, findChannelsById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(user);

    const auto [foundChannel] = findChannelByIdQueryHandler.execute({channel->getId()});

    ASSERT_EQ(foundChannel.getId(), channel->getId());
}
