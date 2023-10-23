#include "gtest/gtest.h"

#include "faker-cxx/Date.h"
#include "RejectFriendInvitationCommandHandlerImpl.h"
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

class RejectFriendInvitationCommandImplIntegrationTest : public Test
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

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapper =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapper, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    RejectFriendInvitationCommandHandlerImpl rejectFriendInvitationCommandHandler{friendInvitationRepository,
                                                                                  userRepository};
};

TEST_F(RejectFriendInvitationCommandImplIntegrationTest, rejectFriendInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto friendInvitation = friendInvitationTestUtils.createAndPersist(sender, recipient);

    rejectFriendInvitationCommandHandler.execute({recipient->getId(), friendInvitation->getId()});

    const auto foundFriendInvitation = friendInvitationTestUtils.find(sender->getId(), recipient->getId());

    ASSERT_FALSE(foundFriendInvitation);
}
