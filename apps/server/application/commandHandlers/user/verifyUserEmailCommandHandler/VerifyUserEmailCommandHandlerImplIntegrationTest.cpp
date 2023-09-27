#include "gtest/gtest.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "User.odb.h"
#include "VerifyUserEmailCommandHandlerImpl.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::application;

class VerifyUserEmailCommandImplIntegrationTest : public Test
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

    VerifyUserEmailCommandHandlerImpl verifyUserEmailCommandHandler{userRepository};
};

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyNotExistingUser)
{
    const auto email = faker::Internet::email();
    const auto verificationCode = "123";

    ASSERT_THROW(verifyUserEmailCommandHandler.execute({email, verificationCode}), errors::ResourceNotFoundError);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyExistingUserWithInvalidVerificationCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = "123";
    const auto invalidVerificationCode = "12";
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    server::infrastructure::User user{
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, invalidVerificationCode});

    ASSERT_FALSE(verified);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyAlreadyVerifiedUser)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = true;
    const auto verificationCode = "123";
    const auto invalidVerificationCode = "12";
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    server::infrastructure::User user{
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, invalidVerificationCode});

    ASSERT_TRUE(verified);
}

TEST_F(VerifyUserEmailCommandImplIntegrationTest, verifyExistingUserWithValidVerificationCode)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto hashedPassword = "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8";
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = false;
    const auto verificationCode = "123";
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    server::infrastructure::User user{
        id, email, hashedPassword, email, active, emailVerified, verificationCode, createdAt, updatedAt};

    {
        odb::transaction transaction(db->begin());

        db->persist(user);

        transaction.commit();
    }

    const auto [verified] = verifyUserEmailCommandHandler.execute({email, verificationCode});

    ASSERT_TRUE(verified);

    {
        typedef odb::query<User> Query;

        odb::transaction transaction(db->begin());

        std::shared_ptr<User> loggedInUser(db->query_one<User>(Query::id == id));

        transaction.commit();

        ASSERT_TRUE(loggedInUser);
        ASSERT_EQ(loggedInUser->isEmailVerified(), true);
    }
}
