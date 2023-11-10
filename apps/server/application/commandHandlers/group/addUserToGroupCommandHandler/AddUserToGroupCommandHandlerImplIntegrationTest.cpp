#include "gtest/gtest.h"

#include "AddUserToGroupCommandHandlerImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/groupTestUtils/GroupTestUtils.h"
#include "server/tests/utils/userGroupTestUtils/UserGroupTestUtils.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class AddUserToGroupCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<GroupMapper> groupMapper = std::make_shared<GroupMapperImpl>();

    std::shared_ptr<UserGroupMapper> userGroupMapper = std::make_shared<UserGroupMapperImpl>(userMapper, groupMapper);

    std::shared_ptr<domain::UserGroupRepository> userGroupRepository =
        std::make_shared<UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<domain::GroupRepository> groupRepository = std::make_shared<GroupRepositoryImpl>(db, groupMapper);

    AddUserToGroupCommandHandlerImpl addUserToGroupCommandHandler{userGroupRepository, userRepository, groupRepository};
};

TEST_F(AddUserToGroupCommandImplIntegrationTest, addUserToGroup)
{
    const auto user = userTestUtils.createAndPersist();

    const auto group = groupTestUtils.createAndPersist();

    addUserToGroupCommandHandler.execute({user->getId(), group->getId()});

    const auto foundUserGroup = userGroupTestUtils.find(user->getId(), group->getId());

    ASSERT_TRUE(foundUserGroup);
}
