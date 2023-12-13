#include "gtest/gtest.h"

#include "DeleteUserCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapper.h"
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

class DeleteUserCommandImplIntegrationTest : public Test
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

    std::shared_ptr<domain::UserGroupRepository> userGroupRepository =
        std::make_shared<UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<domain::GroupRepository> groupRepository = std::make_shared<GroupRepositoryImpl>(db, groupMapper);

    DeleteUserCommandHandlerImpl deleteUserCommandHandler{userRepository, userGroupRepository, groupRepository};
};

TEST_F(DeleteUserCommandImplIntegrationTest, givenExistingUser_shouldDeleteUserAndHisGroups)
{
    const auto user1 = userTestUtils.createAndPersist();

    const auto user2 = userTestUtils.createAndPersist();

    const auto user3 = userTestUtils.createAndPersist();

    const auto group1 = groupTestUtils.createAndPersist();

    const auto group2 = groupTestUtils.createAndPersist();

    userGroupTestUtils.createAndPersist(user1, group1);
    userGroupTestUtils.createAndPersist(user2, group1);
    userGroupTestUtils.createAndPersist(user2, group2);
    userGroupTestUtils.createAndPersist(user3, group2);

    deleteUserCommandHandler.execute({user1->getId()});

    const auto foundUser = userTestUtils.findById(user1->getId());

    ASSERT_FALSE(foundUser);

    const auto foundUserGroup = userGroupTestUtils.find(user1->getId(), group1->getId());

    ASSERT_FALSE(foundUserGroup);

    const auto foundGroup1 = groupTestUtils.findById(group1->getId());

    ASSERT_FALSE(foundGroup1);

    const auto foundGroup2 = groupTestUtils.findById(group2->getId());

    ASSERT_TRUE(foundGroup2);
}

TEST_F(DeleteUserCommandImplIntegrationTest, givenNonExistingUser_shouldThrow)
{
    const auto userId = faker::String::uuid();

    ASSERT_THROW(deleteUserCommandHandler.execute({userId}), errors::ResourceNotFoundError);
}
