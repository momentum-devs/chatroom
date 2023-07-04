#include "gtest/gtest.h"

#include "../../errors/ResourceNotFoundError.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "LogoutUserCommandHandlerImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class LogoutUserCommandImplIntegrationTest : public Test
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

    LogoutUserCommandHandlerImpl logoutUserCommandHandler{userRepository};
};

TEST_F(LogoutUserCommandImplIntegrationTest, logoutExistingUser)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = true;
    const auto emailVerified = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    server::infrastructure::User user{id, email, password, email, active, emailVerified, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    logoutUserCommandHandler.execute({id});

    {
        typedef odb::query<User> Query;

        odb::transaction transaction(db->begin());

        std::shared_ptr<User> loggedOutUser(db->query_one<User>(Query::id == id));

        transaction.commit();

        ASSERT_TRUE(loggedOutUser);
        ASSERT_EQ(loggedOutUser->isActive(), false);
    }
}

TEST_F(LogoutUserCommandImplIntegrationTest, logoutNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(logoutUserCommandHandler.execute({id}), errors::ResourceNotFoundError);
}
