#include "GroupTestUtils.h"

#include "fmt/format.h"
#include <utility>

#include "Group.odb.h"
#include "server/infrastructure/database/tables/GroupTable.h"

namespace server::tests
{

GroupTestUtils::GroupTestUtils(std::shared_ptr<odb::sqlite::database> databaseClientInit)
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

std::vector<infrastructure::Group> GroupTestUtils::findAll()
{
    typedef odb::result<infrastructure::Group> Result;

    odb::transaction transaction(databaseClient->begin());

    Result result = databaseClient->query<infrastructure::Group>();

    std::vector<infrastructure::Group> allGroups;

    for (const auto& group : result)
    {
        allGroups.push_back(group);
    }

    return allGroups;
}

void GroupTestUtils::truncateTable()
{
    odb::transaction transaction(databaseClient->begin());

    databaseClient->execute(fmt::format("DELETE FROM \"{}\";", infrastructure::groupTableName));

    transaction.commit();
}

}
