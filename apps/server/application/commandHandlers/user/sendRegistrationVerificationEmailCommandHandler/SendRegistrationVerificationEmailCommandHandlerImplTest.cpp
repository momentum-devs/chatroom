#include "SendRegistrationVerificationEmailCommandHandlerImpl.h"

#include "gtest/gtest.h"

#include "server/application/services/emailService/EmailServiceMock.h"
#include "server/domain/repositories/userRepository/UserRepositoryMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

class SendRegistrationVerificationEmailCommandHandlerImplTest : public Test
{
public:
    std::shared_ptr<domain::UserRepositoryMock> userRepository =
        std::make_shared<StrictMock<domain::UserRepositoryMock>>();
    std::shared_ptr<EmailServiceMock> emailService = std::make_shared<StrictMock<EmailServiceMock>>();

    SendRegistrationVerificationEmailCommandHandlerImpl handler{userRepository, emailService};
};

TEST_F(SendRegistrationVerificationEmailCommandHandlerImplTest, givenNotExistingUser_shouldThrow)
{
    const auto email = faker::Internet::email();

    const auto expectedFindUserByEmailPayload = domain::FindUserByEmailPayload{email};

    EXPECT_CALL(*userRepository, findUserByEmail(expectedFindUserByEmailPayload)).WillOnce(Return(std::nullopt));

    ASSERT_THROW(handler.execute({email}), errors::ResourceNotFoundError);
}

TEST_F(SendRegistrationVerificationEmailCommandHandlerImplTest, givenExistingUser_shouldSendEmail)
{
    const auto id = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(8);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user = std::make_shared<server::domain::User>(id, email, password, email, active, emailVerified,
                                                             verificationCode, createdAt, updatedAt);

    const auto expectedFindUserByEmailPayload = domain::FindUserByEmailPayload{email};

    const auto expectedFromAddress = "michal.andrzej.cieslar@gmail.com";

    const auto expectedEmailSubject = "Chatroom Registration Verification";

    const auto expectedSendEmailPayload =
        application::SendEmailPayload{email, expectedFromAddress, expectedEmailSubject, verificationCode};

    EXPECT_CALL(*userRepository, findUserByEmail(expectedFindUserByEmailPayload)).WillOnce(Return(user));
    EXPECT_CALL(*emailService, sendEmail(expectedSendEmailPayload));

    handler.execute({email});
}
