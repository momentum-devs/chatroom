#include "gtest/gtest.h"

#include "faker-cxx/Internet.h"
#include "RegisterUserCommandHandlerImpl.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/passwordValidationService/PasswordValidationServiceImpl.h"
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

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    std::shared_ptr<PasswordValidationService> passwordValidationService =
        std::make_shared<PasswordValidationServiceImpl>();

    RegisterUserCommandHandlerImpl registerUserCommandHandler{userRepository, hashService, passwordValidationService};
};

TEST_F(RegisterUserCommandImplIntegrationTest, registerUser)
{
    const auto email = faker::Internet::email();
    const auto password = "Password123";
    const auto expectedHashPassword = "b2e98ad6f6eb8508dd6a14cfa704bad7f05f6fb1";
    const auto nickname = faker::Internet::username();

    const auto [user] = registerUserCommandHandler.execute({email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), expectedHashPassword);
    ASSERT_EQ(user.getNickname(), nickname);
}

TEST_F(RegisterUserCommandImplIntegrationTest, givenPasswordNotMatchingRequirements_shouldThrow)
{
    const auto email = faker::Internet::email();
    const auto password = "password";
    const auto nickname = faker::Internet::username();

    ASSERT_THROW(registerUserCommandHandler.execute({email, password, nickname}), errors::OperationNotValidError);
}

TEST_F(RegisterUserCommandImplIntegrationTest, givenUserWithSameEmail_shouldThrow)
{
    const auto user = userTestUtils.createAndPersist();

    ASSERT_THROW(registerUserCommandHandler.execute({user->getEmail(), user->getPassword(), user->getNickname()}),
                 errors::ResourceAlreadyExistsError);
}
