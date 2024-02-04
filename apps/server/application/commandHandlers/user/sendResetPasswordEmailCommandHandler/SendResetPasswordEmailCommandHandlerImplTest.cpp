#include "SendResetPasswordEmailCommandHandlerImpl.h"

#include "gtest/gtest.h"

#include "server/application/services/emailService/EmailServiceMock.h"
#include "server/domain/repositories/userRepository/UserRepositoryMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Internet.h"
#include "server/application/errors/ResourceNotFoundError.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"
#include "User.h"

using namespace ::testing;
using namespace server;
using namespace server::application;
using namespace server::tests;

class SendResetPasswordEmailCommandHandlerImplTest : public Test
{
public:
    UserTestFactory userTestFactory;

    std::shared_ptr<domain::UserRepositoryMock> userRepository =
        std::make_shared<StrictMock<domain::UserRepositoryMock>>();
    std::shared_ptr<EmailServiceMock> emailService = std::make_shared<StrictMock<EmailServiceMock>>();

    SendResetPasswordEmailCommandHandlerImpl handler{userRepository, emailService};
};

TEST_F(SendResetPasswordEmailCommandHandlerImplTest, givenNotExistingUser_shouldThrow)
{
    const auto email = faker::Internet::email();

    const auto expectedFindUserByEmailPayload = domain::FindUserByEmailPayload{email};

    EXPECT_CALL(*userRepository, findUserByEmail(expectedFindUserByEmailPayload)).WillOnce(Return(std::nullopt));

    ASSERT_THROW(handler.execute({email}), errors::ResourceNotFoundError);
}

TEST_F(SendResetPasswordEmailCommandHandlerImplTest, givenExistingUser_shouldSendEmail)
{
    const auto user = userTestFactory.createDomainUser();

    const auto expectedFindUserByEmailPayload = domain::FindUserByEmailPayload{user->getEmail()};

    EXPECT_CALL(*userRepository, findUserByEmail(expectedFindUserByEmailPayload)).WillOnce(Return(user));
    EXPECT_CALL(*emailService, sendEmail(_));

    handler.execute({user->getEmail()});
}
