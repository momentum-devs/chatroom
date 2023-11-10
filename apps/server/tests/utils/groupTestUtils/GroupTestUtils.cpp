#include "GroupTestUtils.h"

#include <format>
#include <utility>

#include "Group.odb.h"
#include "server/infrastructure/database/tables/GroupTable.h"

namespace server::tests
{

GroupTestUtils::GroupTestUtils(std::shared_ptr<odb::pgsql::database> databaseClientInit)
    : databaseClient{std::move(databaseClientInit)}, groupTestFactory{std::make_unique<GroupTestFactory>()}
{
}

void GroupTestUtils::persist(const std::shared_ptr<infrastructure::Group>& group)
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->persist(group);

    transaction.commit();
}

std::shared_ptr<infrastructure::Group> GroupTestUtils::createAndPersist()
{
    auto group = groupTestFactory->createPersistentGroup();

    persist(group);

    return group;
}

std::shared_ptr<infrastructure::Group> GroupTestUtils::findById(const std::string& id)
{
    typedef odb::query<infrastructure::Group> query;

    odb::transaction transaction(databaseClient->begin());

    std::shared_ptr<infrastructure::Group> foundGroup(
        databaseClient->query_one<infrastructure::Group>(query::id == id));

    transaction.commit();

    return foundGroup;
}

void GroupTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(std::format("DELETE FROM \"{}\";", infrastructure::groupTableName));

    transaction.commit();
}
}
