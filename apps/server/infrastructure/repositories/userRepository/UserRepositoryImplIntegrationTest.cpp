#include <odb/sqlite/database.hxx>

#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
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
        userTestUtils.truncateTable();
    }

    void TearDown() override
    {
        userTestUtils.truncateTable();
    }

    std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();

    UserTestUtils userTestUtils{db};

    UserTestFactory userTestFactory;

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
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = userRepository->createUser(
        {userId, email, password, nickname, active, emailVerified, verificationCode, resetPasswordCode, avatarUrl});

    ASSERT_EQ(user->getEmail(), email);
    ASSERT_EQ(user->getPassword(), password);
    ASSERT_EQ(user->getNickname(), nickname);
}

TEST_F(UserRepositoryIntegrationTest, shouldDeleteExistingUser)
{
    const auto user = userTestUtils.createAndPersist();

    const auto domainUser = userMapper->mapToDomainUser(user);

    userRepository->deleteUser({*domainUser});

    const auto foundUser = userTestUtils.findById(user->getId());

    ASSERT_FALSE(foundUser);
}

TEST_F(UserRepositoryIntegrationTest, delete_givenNonExistingUser_shouldThrowError)
{
    const auto domainUser = userTestFactory.createDomainUser();

    ASSERT_ANY_THROW(userRepository->deleteUser({*domainUser}));
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserByEmail)
{
    const auto user = userTestUtils.createAndPersist();

    const auto foundUser = userRepository->findUserByEmail({user->getEmail()});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->get()->getEmail(), user->getEmail());
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserByEmail)
{
    const auto email = faker::Internet::email();

    const auto user = userRepository->findUserByEmail({email});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldFindExistingUserById)
{
    const auto user = userTestUtils.createAndPersist();

    const auto foundUser = userRepository->findUserById({user->getId()});

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->get()->getId(), user->getId());
}

TEST_F(UserRepositoryIntegrationTest, givenNonExistingUser_shouldNotFindAnyUserById)
{
    const auto userId = faker::String::uuid();

    const auto user = userRepository->findUserById({userId});

    ASSERT_FALSE(user);
}

TEST_F(UserRepositoryIntegrationTest, shouldUpdateExistingUser)
{
    const auto updatedPassword = faker::Internet::password();
    const auto updatedNickname = faker::Internet::username();
    const auto updatedActive = faker::Datatype::boolean();
    const auto updatedEmailVerified = faker::Datatype::boolean();
    const auto updatedVerificationCode = faker::String::numeric(6);
    const auto updatedResetPasswordCode = faker::String::numeric(6);
    const auto updatedAvatarUrl = faker::Image::imageUrl();

    const auto user = userTestUtils.createAndPersist();

    const auto domainUser = userMapper->mapToDomainUser(user);

    domainUser->setPassword(updatedPassword);
    domainUser->setNickname(updatedNickname);
    domainUser->setActive(updatedActive);
    domainUser->setEmailVerified(updatedEmailVerified);
    domainUser->setVerificationCode(updatedVerificationCode);
    domainUser->setResetPasswordCode(updatedResetPasswordCode);
    domainUser->setAvatarUrl(updatedAvatarUrl);

    userRepository->updateUser({*domainUser});

    const auto updatedUser = userRepository->findUserById({user->getId()});

    ASSERT_TRUE(updatedUser);
    ASSERT_EQ((*updatedUser)->getNickname(), updatedNickname);
    ASSERT_EQ((*updatedUser)->getPassword(), updatedPassword);
    ASSERT_EQ((*updatedUser)->isActive(), updatedActive);
    ASSERT_EQ((*updatedUser)->isEmailVerified(), updatedEmailVerified);
    ASSERT_EQ((*updatedUser)->getVerificationCode(), updatedVerificationCode);
    ASSERT_EQ((*updatedUser)->getResetPasswordCode(), updatedResetPasswordCode);
    ASSERT_EQ((*updatedUser)->getAvatarUrl(), updatedAvatarUrl);
}

TEST_F(UserRepositoryIntegrationTest, update_givenNonExistingUser_shouldThrowError)
{
    const auto domainUser = userTestFactory.createDomainUser();

    ASSERT_ANY_THROW(userRepository->updateUser({*domainUser}));
}
