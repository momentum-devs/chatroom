#include <boost/date_time/posix_time/posix_time.hpp>
#include <utility>

#include "gtest/gtest.h"

#include "DeleteFriendshipCommandHandlerImpl.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "Friendship.h"
#include "Friendship.odb.h"
#include "server/application/errors/OperationNotValidError.h"
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

class DeleteFriendshipCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<FriendshipMapper> friendshipMapperInit = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<domain::FriendshipRepository> friendshipRepository =
        std::make_shared<FriendshipRepositoryImpl>(db, friendshipMapperInit, userMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    DeleteFriendshipCommandHandlerImpl deleteFriendshipCommandHandler{friendshipRepository, userRepository};
};

TEST_F(DeleteFriendshipCommandImplIntegrationTest, deleteFriendship)
{
    const auto friendshipId = faker::String::uuid();

    const auto userId = faker::String::uuid();
    const auto userFriendId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userFriendEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto userFriend = createUser(userFriendId, userFriendEmail, password);

    const auto friendship = createFriendship(friendshipId, user, userFriend);

    deleteFriendshipCommandHandler.execute({user->getId(), userFriend->getId()});

    typedef odb::query<Friendship> FriendshipQuery;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Friendship> foundFriendship(db->query_one<Friendship>(FriendshipQuery::id == friendshipId));

        ASSERT_FALSE(foundFriendship);

        transaction.commit();
    }
}

TEST_F(DeleteFriendshipCommandImplIntegrationTest, throwsAnError_whenFriendshipDoesNotExist)
{
    const auto userId = faker::String::uuid();
    const auto userFriendId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userFriendEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto userFriend = createUser(userFriendId, userFriendEmail, password);

    ASSERT_THROW(deleteFriendshipCommandHandler.execute({userId, userFriendId}), errors::OperationNotValidError);
}
