#include "SendRegistrationVerificationEmailCommandHandlerImpl.h"

#include "gtest/gtest.h"

#include "server/application/services/emailService/EmailServiceMock.h"
#include "server/domain/repositories/userRepository/UserRepositoryMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::application;
using namespace server::tests;

class SendRegistrationVerificationEmailCommandHandlerImplTest : public Test
{
public:
    UserTestFactory userTestFactory;

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
    const auto user = userTestFactory.createDomainUser();

    const auto expectedFindUserByEmailPayload = domain::FindUserByEmailPayload{user->getEmail()};

    const auto expectedEmailSubject = "Chatroom Registration Verification";

    const auto expectedSendEmailPayload =
        application::SendEmailPayload{user->getEmail(), expectedEmailSubject, user->getVerificationCode()};

    EXPECT_CALL(*userRepository, findUserByEmail(expectedFindUserByEmailPayload)).WillOnce(Return(user));
    EXPECT_CALL(*emailService, sendEmail(expectedSendEmailPayload));

    handler.execute({user->getEmail()});
}
