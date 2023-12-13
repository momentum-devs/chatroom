#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "LogoutUserCommandHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class LogoutUserCommandImplIntegrationTest : public Test
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

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    UserTestFactory userTestFactory;

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    LogoutUserCommandHandlerImpl logoutUserCommandHandler{userRepository};
};

TEST_F(LogoutUserCommandImplIntegrationTest, logoutExistingUser)
{
    auto user = userTestFactory.createPersistentUser();

    user->setActive(true);

    userTestUtils.persist(user);

    logoutUserCommandHandler.execute({user->getId()});

    const auto foundUser = userTestUtils.findById(user->getId());

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->isActive(), false);
}

TEST_F(LogoutUserCommandImplIntegrationTest, logoutNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(logoutUserCommandHandler.execute({id}), errors::ResourceNotFoundError);
}
