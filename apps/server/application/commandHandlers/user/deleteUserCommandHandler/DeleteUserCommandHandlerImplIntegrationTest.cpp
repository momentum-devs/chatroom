#include "gtest/gtest.h"

#include "DeleteUserCommandHandlerImpl.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
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
        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    DeleteUserCommandHandlerImpl deleteUserCommandHandler{userRepository};
};

TEST_F(DeleteUserCommandImplIntegrationTest, givenExistingUser_shouldDeleteUser)
{
    const auto user = userTestUtils.createAndPersist();

    deleteUserCommandHandler.execute({user->getId()});

    const auto foundUser = userTestUtils.findById(user->getId());

    ASSERT_FALSE(foundUser);
}

TEST_F(DeleteUserCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto userId = faker::String::uuid();

    ASSERT_THROW(deleteUserCommandHandler.execute({userId}), errors::ResourceNotFoundError);
}
