#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "RegisterUserCommandHandlerImpl.h"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.odb.h"

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
    const auto email = faker::Internet::email();
    const auto password = "password";
    const auto expectedHashPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";

    const auto [user] = registerUserCommandHandler.execute({email, password});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), expectedHashPassword);
}

TEST_F(RegisterUserCommandImplIntegrationTest, givenUserWithSameEmail_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(8);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    infrastructure::User existingUser{id,        email,    password, nickname, active, emailVerified, verificationCode,
                                      createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(existingUser);

        transaction.commit();
    }

    ASSERT_THROW(registerUserCommandHandler.execute({email, password}), errors::ResourceAlreadyExistsError);
}
