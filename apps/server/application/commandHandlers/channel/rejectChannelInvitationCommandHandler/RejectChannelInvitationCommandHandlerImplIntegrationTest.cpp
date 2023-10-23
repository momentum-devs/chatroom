#include "gtest/gtest.h"

#include "RejectChannelInvitationCommandHandlerImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class RejectChannelInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelInvitationTestUtils.truncateTable();

        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelInvitationTestUtils.truncateTable();

        userChannelTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    UserChannelTestUtils userChannelTestUtils{db};
    ChannelInvitationTestUtils channelInvitationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapperInit =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<ChannelInvitationRepositoryImpl>(db, channelInvitationMapperInit, userMapper, channelMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    RejectChannelInvitationCommandHandlerImpl rejectChannelInvitationCommandHandler{channelInvitationRepository,
                                                                                    userRepository};
};

TEST_F(RejectChannelInvitationCommandImplIntegrationTest, rejectChannelInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationTestUtils.createAndPersist(sender, recipient, channel);

    rejectChannelInvitationCommandHandler.execute({recipient->getId(), channelInvitation->getId()});

    const auto foundChannelInvitation = channelInvitationTestUtils.findById(channelInvitation->getId());

    ASSERT_FALSE(foundChannelInvitation);

    const auto foundUserChannel = userChannelTestUtils.find(recipient->getId(), channel->getId());

    ASSERT_FALSE(foundUserChannel);
}
