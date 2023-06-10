#include <orm/db.hpp>

#include "gtest/gtest.h"

#include "../../errors/UserNotFoundError.h"
#include "LoginUserCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenService.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;

namespace
{
const std::string databaseHost = "localhost";
const std::string databaseName = "chatroom";
const std::string databaseUsername = "local";
const std::string databasePassword = "local";
const auto jwtSecret = "12321313423565365654546654121890008";
const auto jwtExpiresIn = 86400;
}

class LoginUserCommandImplIntegrationTest : public Test
{
public:
    static void SetUpTestSuite()
    {
        server::infrastructure::DatabaseManagerFactory::create(
            {databaseHost, databaseName, databaseUsername, databasePassword});
    }

    void SetUp() override
    {
        Orm::DB::table("users")->truncate();
    }

    void TearDown() override
    {
        Orm::DB::table("users")->truncate();
    }

    std::unique_ptr<UserMapper> userMapperInit = std::make_unique<UserMapperImpl>();

    std::shared_ptr<UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(std::move(userMapperInit));

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    std::shared_ptr<TokenService> tokenService = std::make_shared<TokenServiceImpl>(jwtSecret, jwtExpiresIn);

    LoginUserCommandHandlerImpl loginUserCommandHandler{userRepository, hashService, tokenService};
};

TEST_F(LoginUserCommandImplIntegrationTest, loginExistingUserWithValidPassword)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";

    const auto user = Models::User::create({{"email", QString::fromStdString(email)},
                                            {"password", QString::fromStdString(hashedPassword)},
                                            {"nickname", QString::fromStdString(email)}});

    const auto userId = user.getAttributeValue("id");

    const auto [token] = loginUserCommandHandler.execute({email, password});

    ASSERT_EQ(tokenService->getUserIdFromToken(token), userId);
}

TEST_F(LoginUserCommandImplIntegrationTest, loginExistingUserWithInvalidPassword)
{
    const auto email = "email@example.com";
    const auto password = "invalidPassword";
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";

    Models::User::create({{"email", QString::fromStdString(email)},
                          {"password", QString::fromStdString(hashedPassword)},
                          {"nickname", QString::fromStdString(email)}});

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::UserNotFoundError);
}

TEST_F(LoginUserCommandImplIntegrationTest, loginNotExistingUser)
{
    const auto email = "email@example.com";
    const auto password = "password";

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::UserNotFoundError);
}
