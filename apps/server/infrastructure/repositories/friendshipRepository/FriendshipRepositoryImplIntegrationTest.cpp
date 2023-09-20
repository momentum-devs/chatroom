#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "Friendship.h"
#include "Friendship.odb.h"
#include "FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class FriendshipRepositoryIntegrationTest : public Test
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

    std::shared_ptr<FriendshipMapper> friendshipMapper = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::FriendshipRepository> friendshipRepository =
        std::make_shared<server::infrastructure::FriendshipRepositoryImpl>(db, friendshipMapper, userMapper);
};

TEST_F(FriendshipRepositoryIntegrationTest, shouldCreateFriendship)
{
    const auto friendshipId = faker::String::uuid();

    const auto userId = faker::String::uuid();
    const auto userFriendId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userFriendEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto userFriend = createUser(userFriendId, userFriendEmail, password);

    const auto friendship = friendshipRepository->createFriendship(
        {friendshipId, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(userFriend)});

    ASSERT_EQ(friendship.getUser()->getId(), userId);
    ASSERT_EQ(friendship.getUserFriend()->getId(), userFriendId);

    typedef odb::query<Friendship> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Friendship> foundFriendship(db->query_one<Friendship>(query::id == friendshipId));

        ASSERT_TRUE(foundFriendship);

        transaction.commit();
    }
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldDeleteExistingFriendship)
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

    const auto domainFriendship = domain::Friendship{friendship.getId(), userMapper->mapToDomainUser(user),
                                                     userMapper->mapToDomainUser(userFriend), friendship.getCreatedAt(),
                                                     friendship.getUpdatedAt()};

    friendshipRepository->deleteFriendship({domainFriendship});

    typedef odb::query<Friendship> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<Friendship> foundFriendship(db->query_one<Friendship>(query::id == friendshipId));

        ASSERT_FALSE(foundFriendship);

        transaction.commit();
    }
}

TEST_F(FriendshipRepositoryIntegrationTest, delete_givenNonExistingFriendship_shouldThrowError)
{
    const auto friendshipId = faker::String::uuid();
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto userId = faker::String::uuid();
    const auto userFriendId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userFriendEmail = faker::Internet::email();
    const auto password = faker::Internet::password();

    const auto user = createUser(userId, userEmail, password);

    const auto userFriend = createUser(userFriendId, userFriendEmail, password);

    const auto domainFriendship = domain::Friendship{friendshipId, userMapper->mapToDomainUser(user),
                                                     userMapper->mapToDomainUser(userFriend), createdAt, updatedAt};

    ASSERT_ANY_THROW(friendshipRepository->deleteFriendship({domainFriendship}));
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldFindFriendshipById)
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

    const auto foundFriendship = friendshipRepository->findFriendshipById({friendshipId});

    ASSERT_TRUE(foundFriendship);
    ASSERT_EQ(foundFriendship->getId(), friendshipId);
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldFindFriendshipsByUserId)
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

    const auto foundFriendships = friendshipRepository->findFriendshipsByUserId({userFriendId});

    ASSERT_EQ(foundFriendships.size(), 1);
    ASSERT_EQ(foundFriendships[0].getId(), friendship.getId());
}
