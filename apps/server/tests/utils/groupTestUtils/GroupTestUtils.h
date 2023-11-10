#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "Group.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.h"

namespace server::tests
{
class GroupTestUtils
{
public:
    explicit GroupTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::Group>& group);
    std::shared_ptr<infrastructure::Group> createAndPersist();
    std::shared_ptr<infrastructure::Group> findById(const std::string& id);
    std::vector<infrastructure::Group> findAll();
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<GroupTestFactory> groupTestFactory;
};
}
