#include "gtest/gtest.h"

#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandler.h"
#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandlerImpl.h"
#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "UserRepositoryImpl.h"

using namespace ::testing;
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

    std::unique_ptr<server::infrastructure::UserMapper> userMapper =
        std::make_unique<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(std::move(userMapper));
};

TEST_F(UserRepositoryIntegrationTest, createUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = userRepository->createUser({email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), password);
    ASSERT_EQ(user.getNickname(), nickname);
}
