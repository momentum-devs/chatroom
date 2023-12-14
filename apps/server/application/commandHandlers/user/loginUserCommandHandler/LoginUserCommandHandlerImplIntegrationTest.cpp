#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "LoginUserCommandHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImpl.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/BlacklistTokenRepositoryImpl.h"
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

const auto jwtSecret = "12321313423565365654546654121890008";
const auto jwtExpiresIn = 86400;

class LoginUserCommandImplIntegrationTest : public Test
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

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    std::shared_ptr<BlacklistTokenMapper> blacklistTokenMapper = std::make_shared<BlacklistTokenMapperImpl>();

    std::shared_ptr<BlacklistTokenRepository> blacklistTokenRepository =
        std::make_shared<BlacklistTokenRepositoryImpl>(db, blacklistTokenMapper);

    std::shared_ptr<TokenService> tokenService =
        std::make_shared<TokenServiceImpl>(jwtSecret, jwtExpiresIn, blacklistTokenRepository);

    LoginUserCommandHandlerImpl loginUserCommandHandler{userRepository, hashService, tokenService};
};

TEST_F(LoginUserCommandImplIntegrationTest, loginExistingUserWithValidPassword)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = "password";
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(8);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<server::infrastructure::User>(
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt);

    userTestUtils.persist(user);

    const auto [token] = loginUserCommandHandler.execute({email, password});

    const auto foundUser = userTestUtils.findById(user->getId());

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->isActive(), true);

    auto expectedVerifyTokenResult = VerifyTokenResult{user->getId()};

    ASSERT_EQ(tokenService->verifyToken(token), expectedVerifyTokenResult);
}

TEST_F(LoginUserCommandImplIntegrationTest, loginExistingUserWithInvalidPassword)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = "invalidPassword";
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(8);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<server::infrastructure::User>(
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt);

    userTestUtils.persist(user);

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::ResourceNotFoundError);
}

TEST_F(LoginUserCommandImplIntegrationTest, loginNotExistingUser)
{
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::ResourceNotFoundError);
}
