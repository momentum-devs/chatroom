#include <orm/db.hpp>

#include "gtest/gtest.h"

#include "RegisterUserCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;

class RegisterUserCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserRepository> userRepository =
        std::make_shared<UserRepositoryImpl>(db, std::move(userMapperInit));

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    RegisterUserCommandHandlerImpl registerUserCommandHandler{userRepository, hashService};
};

TEST_F(RegisterUserCommandImplIntegrationTest, registerUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto expectedHashPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";

    const auto [user] = registerUserCommandHandler.execute({email, password});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), expectedHashPassword);
}
