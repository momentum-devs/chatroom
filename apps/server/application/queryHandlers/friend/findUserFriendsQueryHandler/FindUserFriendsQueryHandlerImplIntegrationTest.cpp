#include "gtest/gtest.h"

#include "FindUserFriendsQueryHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindUserFriendsQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        friendshipTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    FriendshipTestUtils friendshipTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    FindUserFriendsQueryHandlerImpl findUserFriendsQueryHandler{friendshipRepository};
};

TEST_F(FindUserFriendsQueryHandlerImplIntegrationTest, findUsersChannelsByUserId)
{
    const auto user1 = userTestUtils.createAndPersist();

    const auto user2 = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    friendshipTestUtils.createAndPersist(userFriend, user1);
    friendshipTestUtils.createAndPersist(userFriend, user2);
    friendshipTestUtils.createAndPersist(user1, user2);

    const auto [friends] = findUserFriendsQueryHandler.execute({user1->getId()});

    ASSERT_EQ(friends.size(), 2);
    ASSERT_TRUE(std::any_of(friends.begin(), friends.end(),
                            [userFriend](const auto& currentFriend)
                            { return userFriend->getId() == currentFriend.getId(); }));
    ASSERT_TRUE(std::any_of(friends.begin(), friends.end(),
                            [user2](const auto& currentFriend) { return user2->getId() == currentFriend.getId(); }));
}
