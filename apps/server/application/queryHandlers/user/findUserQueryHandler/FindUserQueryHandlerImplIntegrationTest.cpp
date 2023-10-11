#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "FindUserQueryHandlerImpl.h"
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

class FindUserQueryHandlerImplIntegrationTest : public Test
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

    FindUserQueryHandlerImpl findUserQueryHandler{userRepository};
};

TEST_F(FindUserQueryHandlerImplIntegrationTest, findNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(findUserQueryHandler.execute({id}), errors::ResourceNotFoundError);
}

TEST_F(FindUserQueryHandlerImplIntegrationTest, findUserById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto [foundUser] = findUserQueryHandler.execute({user->getId()});

    ASSERT_EQ(foundUser.getId(), user->getId());
}
