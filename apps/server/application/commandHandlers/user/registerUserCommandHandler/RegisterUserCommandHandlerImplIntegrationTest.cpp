#include "gtest/gtest.h"

#include "faker-cxx/Internet.h"
#include "RegisterUserCommandHandlerImpl.h"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;
using namespace server::tests;

class RegisterUserCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    RegisterUserCommandHandlerImpl registerUserCommandHandler{userRepository, hashService};
};

TEST_F(RegisterUserCommandImplIntegrationTest, registerUser)
{
    const auto email = faker::Internet::email();
    const auto password = "password";
    const auto expectedHashPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";

    const auto [user] = registerUserCommandHandler.execute({email, password});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), expectedHashPassword);
}

TEST_F(RegisterUserCommandImplIntegrationTest, givenUserWithSameEmail_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    ASSERT_THROW(registerUserCommandHandler.execute({user->getEmail(), user->getPassword()}),
                 errors::ResourceAlreadyExistsError);
}
