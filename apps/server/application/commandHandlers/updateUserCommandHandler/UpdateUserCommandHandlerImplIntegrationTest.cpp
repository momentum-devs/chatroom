#include "gtest/gtest.h"

#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "UpdateUserCommandHandlerImpl.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::domain;

class UpdateUserCommandImplIntegrationTest : public Test
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

    UpdateUserCommandHandlerImpl updateUserCommandHandler{userRepository};
};

TEST_F(UpdateUserCommandImplIntegrationTest, updateNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto password = faker::Internet::password();

    ASSERT_THROW(updateUserCommandHandler.execute({id, std::nullopt, password}), errors::ResourceNotFoundError);
}

TEST_F(UpdateUserCommandImplIntegrationTest, updatePassword)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto updatedPassword = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = true;
    const auto emailVerified = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    infrastructure::User existingUser{id, email, password, nickname, active, emailVerified, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(existingUser);

        transaction.commit();
    }

    const auto [user] = updateUserCommandHandler.execute({id, std::nullopt, updatedPassword});

    ASSERT_EQ(user.getId(), id);
    ASSERT_EQ(user.getPassword(), updatedPassword);
}

TEST_F(UpdateUserCommandImplIntegrationTest, updateNickname)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto updatedNickname = faker::Internet::username();
    const auto active = true;
    const auto emailVerified = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    infrastructure::User existingUser{id, email, password, nickname, active, emailVerified, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(existingUser);

        transaction.commit();
    }

    const auto [user] = updateUserCommandHandler.execute({id, updatedNickname, std::nullopt});

    ASSERT_EQ(user.getId(), id);
    ASSERT_EQ(user.getNickname(), updatedNickname);
}
