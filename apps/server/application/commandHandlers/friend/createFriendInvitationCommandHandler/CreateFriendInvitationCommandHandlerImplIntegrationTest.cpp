#include "gtest/gtest.h"

#include "CreateFriendInvitationCommandHandlerImpl.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class CreateFriendInvitationCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendInvitationTestUtils.truncateTable();

        friendshipTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};
    FriendInvitationTestUtils friendInvitationTestUtils{db};
    GroupTestUtils groupTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<FriendInvitationMapper> friendInvitationMapperInit =
        std::make_shared<FriendInvitationMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendInvitationRepository> friendInvitationRepository =
        std::make_shared<FriendInvitationRepositoryImpl>(db, friendInvitationMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit =
        std::make_shared<FriendshipMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper, groupMapper);

    CreateFriendInvitationCommandHandlerImpl createFriendInvitationCommandHandler{friendInvitationRepository,
                                                                                  userRepository, friendshipRepository};
};

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, createFriendInvitation)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()});

    const auto foundFriendInvitation = friendInvitationTestUtils.find(sender->getId(), recipient->getId());

    ASSERT_TRUE(foundFriendInvitation);
}

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, throwsAnError_whenRecipientAndSenderIsSameUser)
{
    const auto sender = userTestUtils.createAndPersist();

    friendInvitationTestUtils.createAndPersist(sender, sender);

    ASSERT_THROW(createFriendInvitationCommandHandler.execute({sender->getId(), sender->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, throwsAnError_whenFriendInvitationAlreadyExists)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    friendInvitationTestUtils.createAndPersist(sender, recipient);

    ASSERT_THROW(createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()}),
                 errors::OperationNotValidError);
}

TEST_F(CreateFriendInvitationCommandImplIntegrationTest, throwsAnError_whenFriendshipAlreadyExists)
{
    const auto sender = userTestUtils.createAndPersist();

    const auto recipient = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    friendshipTestUtils.createAndPersist(sender, recipient, group);

    ASSERT_THROW(createFriendInvitationCommandHandler.execute({sender->getId(), recipient->getId()}),
                 errors::OperationNotValidError);
}
