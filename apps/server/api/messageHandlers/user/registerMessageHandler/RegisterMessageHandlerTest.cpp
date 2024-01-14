#include "RegisterMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application//commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerMock.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerMock.h"

#include "nlohmann/json.hpp"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
nlohmann::json validMessageResponsePayloadJson{"ok"};
common::messages::Message validMessageResponse{common::messages::MessageId::RegisterResponse,
                                               common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

nlohmann::json invalidEmailMessageResponsePayloadJson{{"error", "wrong email address"}};
common::messages::Message invalidEmailMessageResponse{
    common::messages::MessageId::RegisterResponse, common::bytes::Bytes{invalidEmailMessageResponsePayloadJson.dump()}};

std::runtime_error registerUserError("registerUserError");
common::messages::Message registerUserErrorMessageResponse{common::messages::MessageId::RegisterResponse,
                                                           common::bytes::Bytes{R"({"error":"registerUserError"})"}};
}

class RegisterMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    std::shared_ptr<server::domain::User> user = userTestFactory.createDomainUser();
    nlohmann::json validPayloadJson{
        {"email", user->getEmail()}, {"nickname", user->getNickname()}, {"password", user->getPassword()}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message message{common::messages::MessageId::Register, validPayload};
    std::string invalidUserEmail = "invalidUserEmail";
    nlohmann::json payloadJsonWithInvalidEmail{{"email", invalidUserEmail}, {"password", user->getPassword()}};
    common::bytes::Bytes payloadWithInvalidEmail{payloadJsonWithInvalidEmail.dump()};
    common::messages::Message messageWithInvalidEmail{common::messages::MessageId::Register, payloadWithInvalidEmail};
    std::unique_ptr<server::application::RegisterUserCommandHandlerMock> registerUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::RegisterUserCommandHandlerMock>>();
    server::application::RegisterUserCommandHandlerMock* registerUserCommandHandlerMock =
        registerUserCommandHandlerMockInit.get();
    server::application::RegisterUserCommandHandlerPayload commandPayload{user->getEmail(), user->getPassword(),
                                                                          user->getNickname()};

    std::shared_ptr<server::application::SendRegistrationVerificationEmailCommandHandlerMock>
        sendRegistrationVerificationEmailCommandHandlerMock =
            std::make_shared<StrictMock<server::application::SendRegistrationVerificationEmailCommandHandlerMock>>();

    RegisterMessageHandler registerMessageHandler{std::move(registerUserCommandHandlerMockInit),
                                                  sendRegistrationVerificationEmailCommandHandlerMock};
};

TEST_F(RegisterMessageHandlerTest, handleValidRegisterUserMessage)
{
    EXPECT_CALL(*registerUserCommandHandlerMock, execute(commandPayload))
        .WillOnce(Return(server::application::RegisterUserCommandHandlerResult{*user}));

    EXPECT_CALL(*sendRegistrationVerificationEmailCommandHandlerMock,
                execute(server::application::SendRegistrationVerificationEmailCommandHandlerPayload{user->getEmail()}));

    auto responseMessage = registerMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(RegisterMessageHandlerTest, handleRegisterUserMessageWithInvalidEmail)
{
    auto responseMessage = registerMessageHandler.handleMessage(messageWithInvalidEmail);

    EXPECT_EQ(responseMessage, invalidEmailMessageResponse);
}

TEST_F(RegisterMessageHandlerTest, handleRegisterUserMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*registerUserCommandHandlerMock, execute(commandPayload)).WillOnce(Throw(registerUserError));

    auto responseMessage = registerMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, registerUserErrorMessageResponse);
}
