#include "gtest/gtest.h"

#include "Channel.h"
#include "Channel.odb.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FindUserQueryHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindUserQueryHandlerImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    FindUserQueryHandlerImpl findUserQueryHandler{userRepository};
};

TEST_F(FindUserQueryHandlerImplIntegrationTest, findNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(findUserQueryHandler.execute({id}), errors::ResourceNotFoundError);
}

TEST_F(FindUserQueryHandlerImplIntegrationTest, findUserById)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto updatedPassword = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = false;
    const auto createdAt = "2023-06-16";
    const auto updatedAt = "2023-06-16";

    infrastructure::User existingUser{id, email, password, nickname, active, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(existingUser);

        transaction.commit();
    }

    const auto [user] = findUserQueryHandler.execute({id});

    ASSERT_EQ(user.getId(), id);
}
