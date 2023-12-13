#include "gtest/gtest.h"

#include "FindUsersBelongingToGroupQueryHandlerImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/userGroupTestUtils/UserGroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class FindUsersBelongingToGroupQueryHandlerImplIntegrationTest : public Test
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

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};
    GroupTestUtils groupTestUtils{db};
    UserGroupTestUtils userGroupTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<UserGroupMapper> userGroupMapper = std::make_shared<UserGroupMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::UserGroupRepository> groupRepository =
        std::make_shared<UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    FindUsersBelongingToGroupQueryHandlerImpl findUsersBelongingToGroupQueryHandler{groupRepository};
};

TEST_F(FindUsersBelongingToGroupQueryHandlerImplIntegrationTest, findUsersByGroupId)
{
    const auto user1 = userTestUtils.createAndPersist();

    const auto user2 = userTestUtils.createAndPersist();

    const auto group1 = groupTestUtils.createAndPersist();

    const auto group2 = groupTestUtils.createAndPersist();

    userGroupTestUtils.createAndPersist(user1, group1);

    userGroupTestUtils.createAndPersist(user2, group2);

    const auto [users] = findUsersBelongingToGroupQueryHandler.execute({group2->getId()});

    ASSERT_EQ(users.size(), 1);
    ASSERT_EQ(users[0].getId(), user2->getId());
}
