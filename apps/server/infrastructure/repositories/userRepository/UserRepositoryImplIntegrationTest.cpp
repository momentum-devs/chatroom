#include <odb/pgsql/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "User.odb.h"
#include "UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class UserRepositoryIntegrationTest : public Test
{
public:
    void SetUp() override
    {
        userTestUtils->truncateTable();
    }

    void TearDown() override
    {
        userTestUtils->truncateTable();
    }

    std::shared_ptr<odb::pgsql::database> db = DatabaseClientTestFactory::create();

    std::unique_ptr<UserTestUtils> userTestUtils = std::make_unique<UserTestUtils>(db);

    std::shared_ptr<server::infrastructure::UserMapper> userMapper =
        std::make_shared<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(db, userMapper);
};

TEST_F(UserRepositoryIntegrationTest, shouldCreateUser)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user =
        userRepository->createUser({userId, email, password, nickname, active, emailVerified, verificationCode});

    ASSERT_EQ(user->getEmail(), email);
    ASSERT_EQ(user->getPassword(), password);
    ASSERT_EQ(user->getNickname(), nickname);
}

TEST_F(UserRepositoryIntegrationTest, shouldDeleteExistingUser)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    User user{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto domainUser =
        domain::User{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    userRepository->deleteUser({domainUser});

    typedef odb::query<User> query;

    {
        odb::transaction transaction(db->begin());

        std::shared_ptr<User> foundUser(db->query_one<User>(query::id == userId));

        ASSERT_FALSE(foundUser);

        transaction.commit();
    }
}

TEST_F(UserRepositoryIntegrationTest, delete_givenNonExistingUser_shouldThrowError)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto domainUser =
        domain::User{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    ASSERT_ANY_THROW(userRepository->deleteUser({domainUser}));
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserByEmail)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    User user{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundUser = userRepository->findUserByEmail({email});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->get()->getEmail(), email);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserByEmail)
{
    const auto email = faker::Internet::email();

    const auto user = userRepository->findUserByEmail({email});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserById)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    User user{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto foundUser = userRepository->findUserById({userId});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->get()->getId(), userId);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserById)
{
    const auto userId = faker::String::uuid();

    const auto user = userRepository->findUserById({userId});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldUpdateExistingUser)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto updatedPassword = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto updatedNickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto updatedActive = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto updatedEmailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto updatedVerificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    User user{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    auto domainUser =
        domain::User{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    domainUser.setPassword(updatedPassword);
    domainUser.setNickname(updatedNickname);
    domainUser.setActive(updatedActive);
    domainUser.setEmailVerified(updatedEmailVerified);
    domainUser.setVerificationCode(updatedVerificationCode);

    userRepository->updateUser({domainUser});

    {
        typedef odb::query<User> query;

        odb::transaction transaction(db->begin());

        std::shared_ptr<User> updatedUser(db->query_one<User>(query::id == userId));

        transaction.commit();

        ASSERT_TRUE(updatedUser);
        ASSERT_EQ(updatedUser->getNickname(), updatedNickname);
        ASSERT_EQ(updatedUser->getPassword(), updatedPassword);
        ASSERT_EQ(updatedUser->isActive(), updatedActive);
        ASSERT_EQ(updatedUser->isEmailVerified(), updatedEmailVerified);
        ASSERT_EQ(updatedUser->getVerificationCode(), updatedVerificationCode);
    }
}

TEST_F(UserRepositoryIntegrationTest, update_givenNonExistingUser_shouldThrowError)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto domainUser =
        domain::User{userId, email, password, nickname, active, emailVerified, verificationCode, createdAt, updatedAt};

    ASSERT_ANY_THROW(userRepository->updateUser({domainUser}));
}
