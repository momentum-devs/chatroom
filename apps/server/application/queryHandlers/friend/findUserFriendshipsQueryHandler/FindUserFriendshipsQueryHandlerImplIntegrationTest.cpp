#include "gtest/gtest.h"

#include "FindUserFriendshipsQueryHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
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

class FindUserFriendshipsQueryHandlerImplIntegrationTest : public Test
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
    GroupTestUtils groupTestUtils{db};
    FriendshipTestUtils friendshipTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit =
        std::make_shared<FriendshipMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper, groupMapper);

    FindUserFriendshipsQueryHandlerImpl findUserFriendshipsQueryHandler{friendshipRepository};
};

TEST_F(FindUserFriendshipsQueryHandlerImplIntegrationTest, findUsersChannelsByUserId)
{
    const auto user1 = userTestUtils.createAndPersist();

    const auto user2 = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    friendshipTestUtils.createAndPersist(userFriend, user1, group);
    friendshipTestUtils.createAndPersist(userFriend, user2, group);
    friendshipTestUtils.createAndPersist(user1, user2, group);

    const auto [friendships] = findUserFriendshipsQueryHandler.execute({user1->getId()});

    ASSERT_EQ(friendships.size(), 2);
    ASSERT_TRUE(std::any_of(friendships.begin(), friendships.end(), [userFriend](const domain::Friendship& friendship)
                            { return userFriend->getId() == friendship.getUser()->getId(); }));
    ASSERT_TRUE(std::any_of(friendships.begin(), friendships.end(), [user2](const domain::Friendship& friendship)
                            { return user2->getId() == friendship.getUserFriend()->getId(); }));
}
