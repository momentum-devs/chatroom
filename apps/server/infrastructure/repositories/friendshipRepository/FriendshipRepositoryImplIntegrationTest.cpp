#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/friendshipTestUtils/FriendshipTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class FriendshipRepositoryIntegrationTest : public Test
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

    UserTestFactory userTestFactory;

    FriendshipTestFactory friendshipTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<FriendshipMapper> friendshipMapper = std::make_shared<FriendshipMapperImpl>(userMapper);

    std::shared_ptr<server::domain::FriendshipRepository> friendshipRepository =
        std::make_shared<server::infrastructure::FriendshipRepositoryImpl>(db, friendshipMapper, userMapper);
};

TEST_F(FriendshipRepositoryIntegrationTest, shouldCreateFriendship)
{
    const auto friendshipId = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto friendship = friendshipRepository->createFriendship(
        {friendshipId, userMapper->mapToDomainUser(user), userMapper->mapToDomainUser(userFriend)});

    ASSERT_EQ(friendship.getUser()->getId(), user->getId());
    ASSERT_EQ(friendship.getUserFriend()->getId(), userFriend->getId());

    const auto foundFriendship = friendshipTestUtils.findById(friendship.getId());

    ASSERT_TRUE(foundFriendship);
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldDeleteExistingFriendship)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto friendship = friendshipTestUtils.createAndPersist(user, userFriend);

    const auto domainFriendship = friendshipMapper->mapToDomainFriendship(*friendship);

    friendshipRepository->deleteFriendship({domainFriendship});

    const auto foundFriendship = friendshipTestUtils.findById(friendship->getId());

    ASSERT_FALSE(foundFriendship);
}

TEST_F(FriendshipRepositoryIntegrationTest, delete_givenNonExistingFriendship_shouldThrowError)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto domainFriendship = friendshipTestFactory.createDomainFriendship(userMapper->mapToDomainUser(user),
                                                                               userMapper->mapToDomainUser(userFriend));

    ASSERT_ANY_THROW(friendshipRepository->deleteFriendship({*domainFriendship}));
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldFindFriendshipById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto friendship = friendshipTestUtils.createAndPersist(user, userFriend);

    const auto foundFriendship = friendshipRepository->findFriendshipById({friendship->getId()});

    ASSERT_TRUE(foundFriendship);
    ASSERT_EQ(foundFriendship->getId(), friendship->getId());
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldFindFriendshipByUserIds)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto friendship = friendshipTestUtils.createAndPersist(user, userFriend);

    const auto foundFriendship1 = friendshipRepository->findFriendshipByUserIds({user->getId(), userFriend->getId()});

    const auto foundFriendship2 = friendshipRepository->findFriendshipByUserIds({userFriend->getId(), user->getId()});

    ASSERT_TRUE(foundFriendship1);
    ASSERT_TRUE(foundFriendship2);
    ASSERT_EQ(foundFriendship1->getId(), friendship->getId());
    ASSERT_EQ(foundFriendship2->getId(), friendship->getId());
}

TEST_F(FriendshipRepositoryIntegrationTest, shouldFindFriendshipsByUserId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto userFriend = userTestUtils.createAndPersist();

    const auto friendship = friendshipTestUtils.createAndPersist(user, userFriend);

    const auto foundFriendships = friendshipRepository->findFriendshipsByUserId({userFriend->getId()});

    ASSERT_EQ(foundFriendships.size(), 1);
    ASSERT_EQ(foundFriendships[0].getId(), friendship->getId());
}
