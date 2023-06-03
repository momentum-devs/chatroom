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

    std::unique_ptr<server::application::CreateUserCommandHandler> createUserCommandHandler =
        std::make_unique<server::application::CreateUserCommandHandlerImpl>(userRepository);
};

TEST_F(UserRepositoryIntegrationTest, createUser)
{
    const auto deserializedResponse = createUserCommandHandler->execute({});

    ASSERT_EQ(deserializedResponse.interval, 900);
    ASSERT_EQ(deserializedResponse.peersEndpoints.size(), 50);
    ASSERT_EQ(deserializedResponse.peersEndpoints[0], peerEndpoint);
}
