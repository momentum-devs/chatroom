#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "FriendInvitation.h"
#include "server/tests/factories/friendInvitationTestFactory/FriendInvitationTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

namespace server::tests
{
class FriendInvitationTestUtils
{
public:
    explicit FriendInvitationTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::FriendInvitation>& friendInvitation);
    std::shared_ptr<infrastructure::FriendInvitation>
    createAndPersist(const std::shared_ptr<infrastructure::User>& sender,
                     const std::shared_ptr<infrastructure::User>& recipient);
    std::shared_ptr<infrastructure::FriendInvitation> findById(const std::string& id);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<FriendInvitationTestFactory> friendInvitationTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
