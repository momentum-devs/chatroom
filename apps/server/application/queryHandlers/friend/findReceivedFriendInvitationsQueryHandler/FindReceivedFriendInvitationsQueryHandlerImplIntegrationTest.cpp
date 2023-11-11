#include "gtest/gtest.h"

#include "FindReceivedFriendInvitationsQueryHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendInvitationTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendInvitationTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    FriendInvitationTestUtils friendInvitationTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    FindReceivedFriendInvitationsQueryHandlerImpl findReceivedFriendInvitationsQueryHandler{friendInvitationRepository};
};

TEST_F(FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest, findFriendInvitationsByRecipientId)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient1 = userTestUtils.createAndPersist();

    const auto recipient2 = userTestUtils.createAndPersist();

    const auto friendInvitation1 = friendInvitationTestUtils.createAndPersist(sender, recipient1);
    friendInvitationTestUtils.createAndPersist(sender, recipient2);

    const auto [friendInvitations] = findReceivedFriendInvitationsQueryHandler.execute({recipient1->getId()});

    ASSERT_EQ(friendInvitations.size(), 1);
    ASSERT_EQ(friendInvitations[0].getId(), friendInvitation1->getId());
}
