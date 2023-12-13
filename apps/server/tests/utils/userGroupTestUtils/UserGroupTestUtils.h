#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

#include "Group.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/userGroupTestFactory/UserGroupTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"
#include "UserGroup.h"

namespace server::tests
{
class UserGroupTestUtils
{
public:
    explicit UserGroupTestUtils(std::shared_ptr<odb::sqlite::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::UserGroup>& userGroup);
    std::shared_ptr<infrastructure::UserGroup> createAndPersist(const std::shared_ptr<infrastructure::User>& user,
                                                                const std::shared_ptr<infrastructure::Group>& group);
    std::shared_ptr<infrastructure::UserGroup> findById(const std::string& id);
    std::shared_ptr<infrastructure::UserGroup> find(const std::string& userId, const std::string& groupId);
    void truncateTable();

private:
    std::shared_ptr<odb::sqlite::database> databaseClient;
    std::unique_ptr<UserGroupTestFactory> userGroupTestFactory;
    std::unique_ptr<GroupTestFactory> groupTestFactory;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
