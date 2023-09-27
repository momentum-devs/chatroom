#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "LoginUserCommandHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

const auto jwtSecret = "12321313423565365654546654121890008";
const auto jwtExpiresIn = 86400;

class LoginUserCommandImplIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    void TearDown() override
    {
        odb::transaction transaction(db->begin());

        db->execute("DELETE FROM \"users\";");

        transaction.commit();
    }

    std::unique_ptr<UserMapper> userMapperInit = std::make_unique<UserMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserRepository> userRepository =
        std::make_shared<UserRepositoryImpl>(db, std::move(userMapperInit));

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    std::shared_ptr<TokenService> tokenService = std::make_shared<TokenServiceImpl>(jwtSecret, jwtExpiresIn);

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

    server::infrastructure::User user{
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto userId = user.getId();

    const auto [token] = loginUserCommandHandler.execute({email, password});

    {
        typedef odb::query<User> Query;

        odb::transaction transaction(db->begin());

        std::shared_ptr<User> loggedInUser(db->query_one<User>(Query::id == id));

        transaction.commit();

        ASSERT_TRUE(loggedInUser);
        ASSERT_EQ(loggedInUser->isActive(), true);
    }

    ASSERT_EQ(tokenService->getUserIdFromToken(token), userId);
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

    server::infrastructure::User user{id,    email,     hashedPassword, email, active, emailVerified,
                                      "123", createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::ResourceNotFoundError);
}

TEST_F(LoginUserCommandImplIntegrationTest, loginNotExistingUser)
{
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();

    ASSERT_THROW(loginUserCommandHandler.execute({email, password}), errors::ResourceNotFoundError);
}
