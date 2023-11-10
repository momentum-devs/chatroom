#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "../../../tests/factories/userTestFactory/UserTestFactory.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/userGroupTestUtils/UserGroupTestUtils.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "UserGroupRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserGroupRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userGroupTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userGroupTestUtils.truncateTable();

        groupTestUtils.truncateTable();

        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    GroupTestUtils groupTestUtils{db};
    UserGroupTestUtils userGroupTestUtils{db};

    UserTestFactory userTestFactory;

    UserGroupTestFactory userGroupTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<UserGroupMapper> userGroupMapper = std::make_shared<UserGroupMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<server::domain::UserGroupRepository> userGroupRepository =
        std::make_shared<server::infrastructure::UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);
};

TEST_F(UserGroupRepositoryIntegrationTest, shouldCreateUserGroup)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userGroup = userGroupRepository->createUserGroup(
        {id, userMapper->mapToDomainUser(user), groupMapper->mapToDomainGroup(group)});

    ASSERT_EQ(userGroup.getUser()->getId(), user->getId());
    ASSERT_EQ(userGroup.getGroup()->getId(), group->getId());
}

TEST_F(UserGroupRepositoryIntegrationTest, shouldDeleteExistingUserGroup)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userGroup = userGroupTestUtils.createAndPersist(user, group);

    const auto domainUserGroup = userGroupMapper->mapToDomainUserGroup(*userGroup);

    userGroupRepository->deleteUserGroup({domainUserGroup});

    const auto foundUserGroup = userGroupTestUtils.findById(userGroup->getId());

    ASSERT_FALSE(foundUserGroup);
}

TEST_F(UserGroupRepositoryIntegrationTest, delete_givenNonExistingUserGroup_shouldThrowError)
{
    const auto id = faker::String::uuid();

    const auto user = userTestUtils.createAndPersist();

    const auto domainUser = userMapper->mapToDomainUser(user);

    const auto group = groupTestUtils.createAndPersist();

    const auto domainGroup = groupMapper->mapToDomainGroup(group);

    const auto domainUserGroup = userGroupTestFactory.createDomainUserGroup(domainUser, domainGroup);

    ASSERT_ANY_THROW(userGroupRepository->deleteUserGroup({*domainUserGroup}));
}

TEST_F(UserGroupRepositoryIntegrationTest, shouldFindUsersGroupsByUserId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userGroup = userGroupTestUtils.createAndPersist(user, group);

    const auto foundUsersGroups = userGroupRepository->findUsersGroupsByUserId({user->getId()});

    ASSERT_TRUE(foundUsersGroups.size());
    ASSERT_EQ(foundUsersGroups[0].getId(), userGroup->getId());
}

TEST_F(UserGroupRepositoryIntegrationTest, shouldFindUsersGroupsByGroupId)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    const auto userGroup = userGroupTestUtils.createAndPersist(user, group);

    const auto foundUsersGroups = userGroupRepository->findUsersGroupsByGroupId({group->getId()});

    ASSERT_TRUE(foundUsersGroups.size());
    ASSERT_EQ(foundUsersGroups[0].getId(), userGroup->getId());
}
