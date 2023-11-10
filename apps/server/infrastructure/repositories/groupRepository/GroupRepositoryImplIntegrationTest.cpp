#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class GroupRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        groupTestUtils.truncateTable();
    }

    void TearDown() override
    {
        groupTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    GroupTestUtils groupTestUtils{db};

    GroupTestFactory groupTestFactory;

    std::shared_ptr<server::infrastructure::GroupMapper> groupMapper =
        std::make_shared<server::infrastructure::GroupMapperImpl>();

    std::shared_ptr<server::domain::GroupRepository> groupRepository =
        std::make_shared<server::infrastructure::GroupRepositoryImpl>(db, groupMapper);
};

TEST_F(GroupRepositoryIntegrationTest, shouldCreateGroup)
{
    const auto groupId = faker::String::uuid();

    const auto group = groupRepository->createGroup({groupId});

    ASSERT_EQ(group->getId(), groupId);
}

TEST_F(GroupRepositoryIntegrationTest, shouldDeleteExistingGroup)
{
    const auto group = groupTestUtils.createAndPersist();

    const auto domainGroup = groupMapper->mapToDomainGroup(group);

    groupRepository->deleteGroup({*domainGroup});

    const auto foundGroup = groupTestUtils.findById(group->getId());

    ASSERT_FALSE(foundGroup);
}

TEST_F(GroupRepositoryIntegrationTest, delete_givenNonExistingGroup_shouldThrowError)
{
    const auto domainGroup = groupTestFactory.createDomainGroup();

    ASSERT_ANY_THROW(groupRepository->deleteGroup({*domainGroup}));
}

TEST_F(GroupRepositoryIntegrationTest, shouldFindExistingGroupById)
{
    const auto group = groupTestUtils.createAndPersist();

    const auto foundGroup = groupRepository->findGroupById({group->getId()});

    ASSERT_TRUE(foundGroup);
    ASSERT_EQ(foundGroup->get()->getId(), group->getId());
}

TEST_F(GroupRepositoryIntegrationTest, givenNonExistingGroup_shouldNotFindAnyGroupById)
{
    const auto groupId = faker::String::uuid();

    const auto group = groupRepository->findGroupById({groupId});

    ASSERT_FALSE(group);
}

TEST_F(GroupRepositoryIntegrationTest, shouldDeleteGroupsByIds)
{
    const auto group1 = groupTestUtils.createAndPersist();
    const auto group2 = groupTestUtils.createAndPersist();
    const auto group3 = groupTestUtils.createAndPersist();

    std::vector<std::string> groupsIds{group1->getId(), group2->getId()};

    groupRepository->deleteGroups({groupsIds});

    const auto foundGroups = groupTestUtils.findAll();

    ASSERT_EQ(foundGroups.size(), 1);
    ASSERT_EQ(foundGroups[0].getId(), group3->getId());

}
