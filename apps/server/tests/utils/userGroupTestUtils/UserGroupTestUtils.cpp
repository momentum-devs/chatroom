#include "UserGroupTestUtils.h"

#include "fmt/format.h"
#include <utility>

#include "server/infrastructure/database/tables/GroupTable.h"
#include "server/infrastructure/database/tables/UserGroupTable.h"
#include "UserGroup.odb.h"

namespace server::tests
{

UserGroupTestUtils::UserGroupTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)},
      userGroupTestFactory{std::make_unique<UserGroupTestFactory>()},
      groupTestFactory{std::make_unique<GroupTestFactory>()},
      userTestFactory{std::make_unique<UserTestFactory>()}
{
}

void UserGroupTestUtils::persist(const std::shared_ptr<infrastructure::UserGroup>& userGroup)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(userGroup);

    transaction.commit();
}

std::shared_ptr<infrastructure::UserGroup>
UserGroupTestUtils::createAndPersist(const std::shared_ptr<infrastructure::User>& userInput,
                                     const std::shared_ptr<infrastructure::Group>& groupInput)
{
    const auto user = userInput ? userInput : userTestFactory->createPersistentUser();

    const auto group = groupInput ? groupInput : groupTestFactory->createPersistentGroup();

    auto userGroup = userGroupTestFactory->createPersistentUserGroup(user, group);

    persist(userGroup);

    return userGroup;
}

std::shared_ptr<infrastructure::UserGroup> UserGroupTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::UserGroup> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::UserGroup> foundUserGroup(
        databaseClient->query_one<infrastructure::UserGroup>(query::id == id));

    transaction.commit();

    return foundUserGroup;
}

std::shared_ptr<infrastructure::UserGroup> UserGroupTestUtils::find(const std::string& userId,
                                                                    const std::string& groupId)
{
    typedef odb::query<infrastructure::UserGroup> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::UserGroup> foundUserGroup(
        databaseClient->query_one<infrastructure::UserGroup>(query::user->id == userId && query::group->id == groupId));

    transaction.commit();

    return foundUserGroup;
}

void UserGroupTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::userGroupTableName));

    transaction.commit();
}
}
