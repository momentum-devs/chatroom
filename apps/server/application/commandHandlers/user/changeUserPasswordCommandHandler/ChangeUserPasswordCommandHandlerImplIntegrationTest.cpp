#include "gtest/gtest.h"

#include "ChangeUserPasswordCommandHandlerImpl.h"
#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/passwordValidationService/PasswordValidationServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class ChangeUserPasswordCommandImplIntegrationTest : public Test
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

    std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();

    std::shared_ptr<domain::UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);

    std::shared_ptr<HashService> hashService = std::make_shared<HashServiceImpl>();

    std::shared_ptr<PasswordValidationService> passwordValidationService =
        std::make_shared<PasswordValidationServiceImpl>();

    ChangeUserPasswordCommandHandlerImpl changeUserPasswordCommandHandler{userRepository, hashService,
                                                                          passwordValidationService};
};

TEST_F(ChangeUserPasswordCommandImplIntegrationTest, resetPasswordNotExistingUser_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto oldPassword = faker::Internet::password();
    const auto newPassword = faker::Internet::password();

    ASSERT_THROW(changeUserPasswordCommandHandler.execute({id, resetPasswordCode, oldPassword, newPassword}),
                 errors::ResourceNotFoundError);
}

TEST_F(ChangeUserPasswordCommandImplIntegrationTest, resetExistingUserPasswordWithInvalidCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto hashedPassword = hashService->hash(password);
    const auto newPassword = faker::Internet::password();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto invalidCode = faker::String::numeric(5);

    const auto user = std::make_shared<server::infrastructure::User>(id, email, hashedPassword, email, active,
                                                                     emailVerified, verificationCode, resetPasswordCode,
                                                                     createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    ASSERT_ANY_THROW(changeUserPasswordCommandHandler.execute({id, invalidCode, password, newPassword}));
}

TEST_F(ChangeUserPasswordCommandImplIntegrationTest, resetExistingUserPasswordWithValidCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto hashedPassword = hashService->hash(password);
    const auto newPassword = password + "2";
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::infrastructure::User>(id, email, hashedPassword, email, active,
                                                                     emailVerified, verificationCode, resetPasswordCode,
                                                                     createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    changeUserPasswordCommandHandler.execute({id, resetPasswordCode, password, newPassword});

    const auto foundUser = userTestUtils.findById(id);

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->getPassword(), hashService->hash(newPassword));
}

TEST_F(ChangeUserPasswordCommandImplIntegrationTest, resetExistingUserPasswordWithSamePassword_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto hashedPassword = hashService->hash(password);
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::infrastructure::User>(id, email, hashedPassword, email, active,
                                                                     emailVerified, verificationCode, resetPasswordCode,
                                                                     createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    ASSERT_THROW(changeUserPasswordCommandHandler.execute({id, resetPasswordCode, password, password}),
                 errors::OperationNotValidError);
}

TEST_F(ChangeUserPasswordCommandImplIntegrationTest, resetExistingUserPasswordWithInvalidOldPassword_shouldThrow)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto hashedPassword = hashService->hash(password);
    const auto newPassword = faker::Internet::password();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto resetPasswordCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::infrastructure::User>(id, email, hashedPassword, email, active,
                                                                     emailVerified, verificationCode, resetPasswordCode,
                                                                     createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    ASSERT_THROW(
        changeUserPasswordCommandHandler.execute({id, resetPasswordCode, faker::Internet::password(), newPassword}),
        errors::OperationNotValidError);
}
