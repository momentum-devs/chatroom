#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Friendship.h"
#include "server/tests/factories/friendshipTestFactory/FriendshipTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

namespace server::tests
{
class FriendshipTestUtils
{
public:
    explicit FriendshipTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Friendship>& friendship);
    std::shared_ptr<infrastructure::Friendship>
    createAndPersist(const std::shared_ptr<infrastructure::User>& user,
                     const std::shared_ptr<infrastructure::User>& userFriend);
    std::shared_ptr<infrastructure::Friendship> findById(const std::string& id);
    std::shared_ptr<infrastructure::Friendship> find(const std::string& userId, const std::string& userFriendId);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<FriendshipTestFactory> friendshipTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
