
#include "gtest/gtest.h"

#include "DeleteUserCommandHandlerImpl.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.h"
#include "User.odb.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class DeleteUserCommandImplIntegrationTest : public Test
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

    User createUser(const std::string& id, const std::string& email, const std::string& password)
    {
        const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

        User user{id, email, password, email, false, false, "123", currentDate, currentDate};

        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();

        return user;
    }

    std::unique_ptr<UserMapper> userMapperInit = std::make_unique<UserMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<domain::UserRepository> userRepository =
        std::make_shared<UserRepositoryImpl>(db, std::move(userMapperInit));

    DeleteUserCommandHandlerImpl deleteUserCommandHandler{userRepository};
};

TEST_F(DeleteUserCommandImplIntegrationTest, givenExistingUser_shouldDeleteUser)
{
    const auto userId = faker::String::uuid();
    const auto userEmail = faker::Internet::email();
    const auto userPassword = faker::Internet::password();

    const auto user = createUser(userId, userEmail, userPassword);

    deleteUserCommandHandler.execute({user.getId()});

    typedef odb::query<User> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<User> foundUser(db->query_one<User>(query::id == userId));

        ASSERT_FALSE(foundUser);

        transaction.commit();
    }
}

TEST_F(DeleteUserCommandImplIntegrationTest, givenNonExistingChannel_shouldThrow)
{
    const auto userId = faker::String::uuid();

    ASSERT_THROW(deleteUserCommandHandler.execute({userId}), errors::ResourceNotFoundError);
}
