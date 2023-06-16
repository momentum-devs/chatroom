#include <odb/pgsql/database.hxx>
#include <orm/db.hpp>

#include "gtest/gtest.h"

#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "User.odb.h"
#include "UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;

class UserRepositoryIntegrationTest : public Test
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

    std::unique_ptr<server::infrastructure::UserMapper> userMapperInit =
        std::make_unique<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<odb::pgsql::database> db =
        std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(db, std::move(userMapperInit));
};

TEST_F(UserRepositoryIntegrationTest, shouldCreateUser)
{
    const auto id = "id1";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = userRepository->createUser({id, email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), password);
    ASSERT_EQ(user.getNickname(), nickname);
}

TEST_F(UserRepositoryIntegrationTest, givenUserWithExistingEmail_shouldThrowError)
{
    const auto id1 = "id1";
    const auto id2 = "id2";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    User user{id1, email, password, nickname};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    ASSERT_THROW(userRepository->createUser({id2, email, password, nickname}), errors::UserRepositoryError);
}

TEST_F(UserRepositoryIntegrationTest, shouldDeleteExistingUser)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    User user{id, email, password, nickname};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto domainUser = domain::User{id, email, password, nickname};

    userRepository->deleteUser({domainUser});

    typedef odb::query<User> query;

    std::shared_ptr<User> foundUser(db->query_one<User>(query::id == id));

    ASSERT_FALSE(foundUser);
}

TEST_F(UserRepositoryIntegrationTest, delete_givenNonExistingUser_shouldThrowError)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto domainUser = domain::User{id, email, password, nickname};

    ASSERT_ANY_THROW(userRepository->deleteUser({domainUser}));
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserByEmail)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    User user{id, email, password, nickname};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundUser = userRepository->findUserByEmail({email});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->getEmail(), email);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserByEmail)
{
    const auto email = "email@example.com";

    const auto user = userRepository->findUserByEmail({email});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserById)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    User user{id, email, password, nickname};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundUser = userRepository->findUserById({id});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->getId(), id);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserById)
{
    const auto id = "id";

    const auto user = userRepository->findUserById({id});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldUpdateExistingUser)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password1";
    const auto updatedPassword = "password2";
    const auto nickname = "nickname1";
    const auto updatedNickname = "nickname2";

    User user{id, email, password, nickname};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    auto domainUser = domain::User{id, email, password, nickname};

    domainUser.setPassword(updatedPassword);
    domainUser.setNickname(updatedNickname);

    userRepository->updateUser({domainUser});

    typedef odb::query<User> query;

    std::shared_ptr<User> updatedUser(db->query_one<User>(query::id == id));

    ASSERT_TRUE(updatedUser);
    ASSERT_EQ(updatedUser->getNickname(), updatedNickname);
    ASSERT_EQ(updatedUser->getPassword(), updatedPassword);
}

TEST_F(UserRepositoryIntegrationTest, update_givenNonExistingUser_shouldThrowError)
{
    const auto id = "id";
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    auto user = domain::User{id, email, password, nickname};

    ASSERT_ANY_THROW(userRepository->updateUser({user}));
}
