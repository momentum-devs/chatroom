#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "FindUserByEmailQueryHandlerImpl.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class FindUserByEmailQueryHandlerImplIntegrationTest : public Test
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

    FindUserByEmailQueryHandlerImpl findUserByEmailQueryHandler{userRepository};
};

TEST_F(FindUserByEmailQueryHandlerImplIntegrationTest, findNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();

    ASSERT_THROW(findUserByEmailQueryHandler.execute({id}), errors::ResourceNotFoundError);
}

TEST_F(FindUserByEmailQueryHandlerImplIntegrationTest, findUserByEmail)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto updatedPassword = faker::Internet::password();
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

    const auto [user] = findUserByEmailQueryHandler.execute({email});

    ASSERT_EQ(user.getId(), id);
}
