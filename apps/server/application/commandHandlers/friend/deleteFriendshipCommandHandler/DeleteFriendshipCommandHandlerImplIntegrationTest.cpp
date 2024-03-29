#include "gtest/gtest.h"

#include "DeleteFriendshipCommandHandlerImpl.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class DeleteFriendshipCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendshipTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendshipTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};
    GroupTestUtils groupTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<FriendshipMapper> friendshipMapperInit =
        std::make_shared<FriendshipMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper, groupMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    DeleteFriendshipCommandHandlerImpl deleteFriendshipCommandHandler{friendshipRepository, userRepository};
};

TEST_F(DeleteFriendshipCommandImplIntegrationTest, deleteFriendship)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    friendshipTestUtils.createAndPersist(user, userFriend, group);

    deleteFriendshipCommandHandler.execute({user->getId(), userFriend->getId()});

    const auto foundFriendship = friendshipTestUtils.find(user->getId(), userFriend->getId());

    ASSERT_FALSE(foundFriendship);
}

TEST_F(DeleteFriendshipCommandImplIntegrationTest, throwsAnError_whenFriendshipDoesNotExist)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    ASSERT_THROW(deleteFriendshipCommandHandler.execute({user->getId(), userFriend->getId()}),
                 errors::OperationNotValidError);
}
