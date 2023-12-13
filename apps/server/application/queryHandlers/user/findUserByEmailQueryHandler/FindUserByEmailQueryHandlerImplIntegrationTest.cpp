#include "gtest/gtest.h"

#include "faker-cxx/String.h"
#include "FindUserByEmailQueryHandlerImpl.h"
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

class FindUserByEmailQueryHandlerImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    FindUserByEmailQueryHandlerImpl findUserByEmailQueryHandler{userRepository};
};

TEST_F(FindUserByEmailQueryHandlerImplIntegrationTest, findNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(findUserByEmailQueryHandler.execute({id}), errors::ResourceNotFoundError);
}

TEST_F(FindUserByEmailQueryHandlerImplIntegrationTest, findUserByEmail)
{
    const auto user = userTestUtils.createAndPersist();

    const auto [foundUser] = findUserByEmailQueryHandler.execute({user->getEmail()});

    ASSERT_EQ(foundUser.getEmail(), user->getEmail());
}
