#include <orm/db.hpp>

#include "gtest/gtest.h"

#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/errors/UserRepositoryError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "UserRepositoryImpl.h"

using namespace ::testing;
using namespace server;
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
    static void SetUpTestSuite()
    {
        server::infrastructure::DatabaseManagerFactory::create(
            {databaseHost, databaseName, databaseUsername, databasePassword});
    }

    void SetUp() override
    {
        Orm::DB::table("users")->truncate();
    }

    void TearDown() override
    {
        Orm::DB::table("users")->truncate();
    }

    std::unique_ptr<server::infrastructure::UserMapper> userMapperInit =
        std::make_unique<server::infrastructure::UserMapperImpl>();

    server::infrastructure::UserMapper* userMapper = userMapperInit.get();

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(std::move(userMapperInit));
};

TEST_F(UserRepositoryIntegrationTest, shouldCreateUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = userRepository->createUser({email, password, nickname});

    ASSERT_EQ(user.getEmail(), email);
    ASSERT_EQ(user.getPassword(), password);
    ASSERT_EQ(user.getNickname(), nickname);
}

TEST_F(UserRepositoryIntegrationTest, givenUserWithExistingEmail_shouldThrowError)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    Models::User::create({{"email", QString::fromStdString(email)},
                          {"password", QString::fromStdString(password)},
                          {"nickname", QString::fromStdString(nickname)}});

    ASSERT_THROW(userRepository->createUser({email, password, nickname}), errors::UserRepositoryError);
}

TEST_F(UserRepositoryIntegrationTest, shouldDeleteExistingUser)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto modelUser = Models::User::create({{"email", QString::fromStdString(email)},
                                                 {"password", QString::fromStdString(password)},
                                                 {"nickname", QString::fromStdString(nickname)}});

    const auto user = userMapper->mapToDomainUser(modelUser);

    userRepository->deleteUser({user});

    const auto foundUser = Models::User::firstWhereEq("email", QString::fromStdString(email));

    ASSERT_FALSE(foundUser);
}

TEST_F(UserRepositoryIntegrationTest, delete_givenNonExistingUser_shouldThrowError)
{
    const unsigned id = 1;
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = domain::User{id, email, password, nickname};

    ASSERT_ANY_THROW(userRepository->deleteUser({user}));
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserByEmail)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    Models::User::create({{"email", QString::fromStdString(email)},
                          {"password", QString::fromStdString(password)},
                          {"nickname", QString::fromStdString(nickname)}});

    const auto user = userRepository->findUserByEmail({email});

    ASSERT_TRUE(user);
    ASSERT_EQ(user->getEmail(), email);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserByEmail)
{
    const auto email = "email@example.com";

    const auto user = userRepository->findUserByEmail({email});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserById)
{
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto userModel = Models::User::create({{"email", QString::fromStdString(email)},
                                                 {"password", QString::fromStdString(password)},
                                                 {"nickname", QString::fromStdString(nickname)}});

    const auto userId = userModel.getAttributeValue("id").toUInt();

    const auto user = userRepository->findUserById({userId});

    ASSERT_TRUE(user);
    ASSERT_EQ(user->getId(), userId);
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserById)
{
    const auto userId = 5;

    const auto user = userRepository->findUserById({userId});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldUpdateExistingUser)
{
    const auto email = "email@example.com";
    const auto password = "password1";
    const auto updatedPassword = "password2";
    const auto nickname = "nickname1";
    const auto updatedNickname = "nickname2";

    const auto modelUser = Models::User::create({{"email", QString::fromStdString(email)},
                                                 {"password", QString::fromStdString(password)},
                                                 {"nickname", QString::fromStdString(nickname)}});

    auto user = userMapper->mapToDomainUser(modelUser);

    user.setPassword(updatedPassword);
    user.setNickname(updatedNickname);

    userRepository->updateUser({user});

    const auto updatedUser = Models::User::firstWhereEq("email", QString::fromStdString(email));

    ASSERT_TRUE(updatedUser);
    ASSERT_EQ(updatedUser->getAttributeValue("nickname").toString().toStdString(), updatedNickname);
    ASSERT_EQ(updatedUser->getAttributeValue("password").toString().toStdString(), updatedPassword);
}

TEST_F(UserRepositoryIntegrationTest, update_givenNonExistingUser_shouldThrowError)
{
    const unsigned id = 1;
    const auto email = "email@example.com";
    const auto password = "password";
    const auto nickname = "nickname";

    const auto user = domain::User{id, email, password, nickname};

    ASSERT_ANY_THROW(userRepository->updateUser({user}));
}
