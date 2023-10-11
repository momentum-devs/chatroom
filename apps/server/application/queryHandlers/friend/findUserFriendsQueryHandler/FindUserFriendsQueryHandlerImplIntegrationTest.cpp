#include "gtest/gtest.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FindUserFriendsQueryHandlerImpl.h"
#include "Friendship.h"
#include "Friendship.odb.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapper.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindUserFriendsQueryHandlerImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friendships\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"friendships\";");
        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    std::shared_ptr<User> createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        auto user = std::make_shared<User>(id, email, password, email, false, false, "123", currentDate, currentDate);

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    Friendship createFriendship(const std::string& id, std::shared_ptr<User> user, std::shared_ptr<User> userFriend)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        Friendship friendship{id, std::move(user), std::move(userFriend), currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(friendship);

        transaction.commit();

        return friendship;
    }

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    FindUserFriendsQueryHandlerImpl findUserFriendsQueryHandler{friendshipRepository};
};

TEST_F(FindUserFriendsQueryHandlerImplIntegrationTest, findUsersChannelsByUserId)
{
    const auto friendshipId1 = faker::String::uuid();
    const auto friendshipId2 = faker::String::uuid();
    const auto friendshipId3 = faker::String::uuid();

    const auto friendId = faker::String::uuid();
    const auto userId1 = faker::String::uuid();
    const auto userId2 = faker::String::uuid();
    const auto friendEmail = faker::Internet::email();
    const auto userEmail1 = faker::Internet::email();
    const auto userEmail2 = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto userFriend = createUser(friendId, friendEmail, password);

    const auto user1 = createUser(userId1, userEmail1, password);

    const auto user2 = createUser(userId2, userEmail2, password);

    createFriendship(friendshipId1, userFriend, user1);
    createFriendship(friendshipId2, userFriend, user2);
    createFriendship(friendshipId3, user1, user2);

    const auto [friends] = findUserFriendsQueryHandler.execute({user1->getId()});

    ASSERT_EQ(friends.size(), 2);
    ASSERT_TRUE(std::any_of(friends.begin(), friends.end(),
                            [userFriend](const auto& currentFriend)
                            { return userFriend->getId() == currentFriend.getId(); }));
    ASSERT_TRUE(std::any_of(friends.begin(), friends.end(),
                            [user2](const auto& currentFriend) { return user2->getId() == currentFriend.getId(); }));
}
