#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

namespace server::tests
{
class UserTestUtils
{
public:
    explicit UserTestUtils(std::shared_ptr<odb::pgsql::database> databaseClient);

    void persist(const std::shared_ptr<infrastructure::User>& user);
    std::shared_ptr<infrastructure::User> createAndPersist();
    std::shared_ptr<infrastructure::User> findById(const std::string& id);
    void truncateTable();

private:
    std::shared_ptr<odb::pgsql::database> databaseClient;
    std::unique_ptr<UserTestFactory> userTestFactory;
};
}
