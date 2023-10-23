#include "gtest/gtest.h"

#include "CreateChannelInvitationCommandHandlerImpl.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.h"
#include "server/tests/utils/channelTestUtils/ChannelTestUtils.h"
#include "server/tests/utils/userChannelTestUtils/UserChannelTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateChannelInvitationCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserChannelMapper> userChannelMapper = std::make_shared<UserChannelMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<ChannelInvitationMapper> channelInvitationMapper =
        std::make_shared<ChannelInvitationMapperImpl>(userMapper, channelMapper);

    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository =
        std::make_shared<ChannelInvitationRepositoryImpl>(db, channelInvitationMapper, userMapper, channelMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<domain::ChannelRepository> channelRepository =
        std::make_shared<ChannelRepositoryImpl>(db, channelMapper, userMapper);

    std::shared_ptr<domain::UserChannelRepository> userChannelRepository =
        std::make_shared<UserChannelRepositoryImpl>(db, userChannelMapper, userMapper, channelMapper);

    CreateChannelInvitationCommandHandlerImpl createChannelInvitationCommandHandler{channelInvitationRepository,
                                                                                    userRepository, channelRepository, userChannelRepository};
};

TEST_F(CreateChannelInvitationCommandImplIntegrationTest, createChannelInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    createChannelInvitationCommandHandler.execute({sender->getId(), recipient->getId(), channel->getId()});

    const auto foundChannelInvitation = channelInvitationTestUtils.find(sender->getId(), recipient->getId(), channel->getId());

    ASSERT_TRUE(foundChannelInvitation);
}

TEST_F(CreateChannelInvitationCommandImplIntegrationTest, throwsError_whenRecipientIsAlreadyMemberOfChannel)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto channel = channelTestUtils.createAndPersist(sender);

    userChannelTestUtils.createAndPersist(recipient, channel);

    ASSERT_THROW(createChannelInvitationCommandHandler.execute({sender->getId(), recipient->getId(), channel->getId()}), errors::OperationNotValidError);
}
