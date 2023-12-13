#include "UserTestUtils.h"

#include <format>
#include <utility>

#include "server/infrastructure/database/tables/UserTable.h"
#include "User.odb.h"

namespace server::tests
{

UserTestUtils::UserTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)}, userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void UserTestUtils::persist(const std::shared_ptr<infrastructure::User>& user)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(user);

    transaction.commit();
}

std::shared_ptr<infrastructure::User> UserTestUtils::createAndPersist()
{
    auto user = userTestFactory->createPersistentUser();

    persist(user);

    return user;
}

std::shared_ptr<infrastructure::User> UserTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::User> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::User> foundUser(databaseClient->query_one<infrastructure::User>(query::id == id));

    transaction.commit();

    return foundUser;
}

void UserTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::userTableName));

    transaction.commit();
}
}
