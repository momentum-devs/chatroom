#include "gtest/gtest.h"

#include "AcceptFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class AcceptFriendInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};
    FriendInvitationTestUtils friendInvitationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();
    std::shared_ptr<ChannelMapper> channelMapper = std::make_shared<ChannelMapperImpl>(userMapper);

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    std::shared_ptr<application::CreateFriendshipCommandHandler> createFriendshipCommandHandler =
        std::make_shared<CreateFriendshipCommandHandlerImpl>(friendshipRepository, userRepository);

    AcceptFriendInvitationCommandHandlerImpl acceptFriendInvitationCommandHandler{
        friendInvitationRepository, userRepository, createFriendshipCommandHandler};
};

TEST_F(AcceptFriendInvitationCommandImplIntegrationTest, acceptFriendInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    acceptFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation->getId()});

    const auto foundFriendInvitation = friendInvitationTestUtils.findById(friendInvitation->getId());

    ASSERT_FALSE(foundFriendInvitation);

    const auto foundFriendship = friendshipTestUtils.find(sender->getId(), recipient->getId());

    ASSERT_TRUE(foundFriendship);
}
