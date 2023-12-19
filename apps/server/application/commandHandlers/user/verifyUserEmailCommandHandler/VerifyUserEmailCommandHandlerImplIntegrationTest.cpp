#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
#include "server/tests/utils/userTestUtils/UserTestUtils.h"
#include "VerifyUserEmailCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;
using namespace server::tests;

class VerifyUserEmailCommandImplIntegrationTest : public Test
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

    VerifyUserEmailCommandHandlerImpl verifyUserEmailCommandHandler{userRepository};
};

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyNotExistingUser)
{
    const auto email = faker::Internet::email();
    const auto verificationCode = faker::String::numeric(6);

    ASSERT_THROW(verifyUserEmailCommandHandler.execute({email, verificationCode}), errors::ResourceNotFoundError);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyExistingUserWithInvalidVerificationCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto invalidVerificationCode = faker::String::numeric(5);

    const auto user = std::make_shared<server::infrastructure::User>(id, email, password, email, active, emailVerified,
                                                                     verificationCode, createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, invalidVerificationCode});

    ASSERT_FALSE(verified);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyAlreadyVerifiedUser)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = true;
    const auto verificationCode = faker::String::numeric(6);
    const auto invalidVerificationCode = faker::String::numeric(5);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::infrastructure::User>(id, email, password, email, active, emailVerified,
                                                                     verificationCode, createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, invalidVerificationCode});

    ASSERT_TRUE(verified);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyExistingUserWithValidVerificationCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::infrastructure::User>(id, email, password, email, active, emailVerified,
                                                                     verificationCode, createdAt, updatedAt, avatarUrl);

    userTestUtils.persist(user);

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, verificationCode});

    ASSERT_TRUE(verified);

    const auto foundUser = userTestUtils.findById(id);

    ASSERT_TRUE(foundUser);
    ASSERT_EQ(foundUser->isEmailVerified(), true);
}
