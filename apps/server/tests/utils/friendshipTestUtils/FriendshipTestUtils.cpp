#include "FriendshipTestUtils.h"

#include <format>
#include <utility>

#include "Friendship.odb.h"
#include "server/infrastructure/database/tables/FriendshipTable.h"

namespace server::tests
{

FriendshipTestUtils::FriendshipTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      friendshipTestFactory{std::make_unique<FriendshipTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void FriendshipTestUtils::persist(const std::shared_ptr<infrastructure::Friendship>& friendship)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(friendship);

    transaction.commit();
}

std::shared_ptr<infrastructure::Friendship>
FriendshipTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& userInput,
                                      const std::shared_ptr<infrastructure::User>& userFriendInput)
{
    const auto user = userInput ? userInput : userTestFactory->createPersistentUser();

    const auto userFriend = userFriendInput ? userFriendInput : userTestFactory->createPersistentUser();

    auto friendship = friendshipTestFactory->createPersistentFriendship(user, userFriend);

    persist(friendship);

    return friendship;
}

std::shared_ptr<infrastructure::Friendship> FriendshipTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Friendship> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Friendship> foundFriendship(
        databaseClient->query_one<infrastructure::Friendship>(query::id == id));

    transaction.commit();

    return foundFriendship;
}

std::shared_ptr<infrastructure::Friendship> FriendshipTestUtils::find(const std::string& userId,
                                                                      const std::string& userFriendId)
{
    typedef odb::query<infrastructure::Friendship> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Friendship> foundFriendship(databaseClient->query_one<infrastructure::Friendship>(
        query::user->id == userId && query::user_friend->id == userFriendId));

    transaction.commit();

    return foundFriendship;
}

void FriendshipTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::friendshipTableName));

    transaction.commit();
}

}
