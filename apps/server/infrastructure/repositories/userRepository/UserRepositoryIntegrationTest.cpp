#include <orm/db.hpp>

#include "gtest/gtest.h"

#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandler.h"
#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandlerImpl.h"
#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

namespace
{
const std::string databaseHost = "localhost";
const std::string databaseName = "chatroom";
const std::string databaseUsername = "local";
const std::string databasePassword = "local";
}

class UserRepositoryIntegrationTest : public Test
{
public:
    UserRepositoryIntegrationTest()
    {
        server::infrastructure::DatabaseManagerFactory::create(
            {databaseHost, databaseName, databaseUsername, databasePassword});
    }

    void SetUp() override
    {
        Orm::DB::table("users")->truncate();
    }

    std::unique_ptr<server::infrastructure::UserMapper> userMapperInit =
        std::make_unique<server::infrastructure::UserMapperImpl>();

    server::infrastructure::UserMapper* userMapper = userMapperInit.get();

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(std::move(userMapperInit));
};

TEST_F(UserRepositoryIntegrationTest, shouldCreateUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = userRepository->createUser({email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), password);
    ASSERT_EQ(user.getNickname(), nickname);
}

TEST_F(UserRepositoryIntegrationTest, givenUserWithExistingEmail_shouldThrowError)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    Models::User::create({{"email", QString::fromStdString(email)},
                          {"password", QString::fromStdString(password)},
                          {"nickname", QString::fromStdString(nickname)}});

    ASSERT_THROW(userRepository->createUser({email, password, nickname}), errors::UserRepositoryError);
}

TEST_F(UserRepositoryIntegrationTest, shouldDeleteExistingUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto modelUser = Models::User::create({{"email", QString::fromStdString(email)},
                                                 {"password", QString::fromStdString(password)},
                                                 {"nickname", QString::fromStdString(nickname)}});

    const auto user = userMapper->mapToDomainUser(modelUser);

    userRepository->deleteUser({user});

    const auto foundUser = Models::User::firstWhereEq("email", QString::fromStdString(email));

    ASSERT_FALSE(foundUser);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotThrowAnyError)
{
    const unsigned id = 1;
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = domain::User{id, email, password, nickname};

    ASSERT_NO_THROW(userRepository->deleteUser({user}));
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    Models::User::create({{"email", QString::fromStdString(email)},
                          {"password", QString::fromStdString(password)},
                          {"nickname", QString::fromStdString(nickname)}});

    const auto user = userRepository->findUser({email});

    ASSERT_TRUE(user);
    ASSERT_EQ(user->getEmail(), email);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUser)
{
    const auto email = "email@example.com";

    const auto user = userRepository->findUser({email});

    ASSERT_FALSE(user);
}
