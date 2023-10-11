#include "gtest/gtest.h"

#include "FindReceivedChannelInvitationsQueryHandlerImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindReceivedChannelInvitationsQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        channelInvitationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        channelInvitationTestUtils.truncateTable();

        channelTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    ChannelTestUtils channelTestUtils{db};
    ChannelInvitationTestUtils channelInvitationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapper =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<server::domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<server::infrastructure::ChannelInvitationRepositoryImpl>(db, channelInvitationMapper,
                                                                                  userMapper, channelMapper);

    FindReceivedChannelInvitationsQueryHandlerImpl findReceivedChannelInvitationsQueryHandler{
        channelInvitationRepository};
};

TEST_F(FindReceivedChannelInvitationsQueryHandlerImplIntegrationTest, findChannelInvitationsByRecipientId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    const auto channelInvitation = channelInvitationTestUtils.createAndPersist(sender, recipient, channel);

    const auto [channelInvitations] = findReceivedChannelInvitationsQueryHandler.execute({recipient->getId()});

    ASSERT_EQ(channelInvitations.size(), 1);
    ASSERT_EQ(channelInvitations[0].getId(), channelInvitation->getId());
}
