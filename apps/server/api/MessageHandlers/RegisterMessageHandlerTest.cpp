#include "RegisterMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application//commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerMock.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
auto userEmail = "userEmail@mail.com";
auto userPassword = "userPassword";
auto userNickname = "userNickname";
auto userIsActive = true;
auto userEmailVerified = true;
auto validPayloadJson = nlohmann::json{{"email", userEmail}, {"password", userPassword}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::Register, validPayload};
auto validMessageResponsePayloadJson = nlohmann::json{"ok"};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::RegisterResponse,
                                                      common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

auto invalidUserEmail = "invalidUserEmail";
auto payloadJsonWithInvalidEmail = nlohmann::json{{"email", invalidUserEmail}, {"password", userPassword}};
auto payloadWithInvalidEmail = common::bytes::Bytes{payloadJsonWithInvalidEmail.dump()};
auto messageWithInvalidEmail =
    common::messages::Message{common::messages::MessageId::Register, payloadWithInvalidEmail};
auto invalidEmailMessageResponsePayloadJson = nlohmann::json{{"error", "wrong email address"}};
auto invalidEmailMessageResponse = common::messages::Message{
    common::messages::MessageId::RegisterResponse, common::bytes::Bytes{invalidEmailMessageResponsePayloadJson.dump()}};

std::runtime_error registerUserError("registerUserError");
auto registerUserErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::RegisterResponse, common::bytes::Bytes{R"({"error":"registerUserError"})"}};
}

class RegisterMessageHandlerTest : public Test
{
public:
    std::unique_ptr<server::application::RegisterUserCommandHandlerMock> registerUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::RegisterUserCommandHandlerMock>>();
    server::application::RegisterUserCommandHandlerMock* registerUserCommandHandlerMock =
        registerUserCommandHandlerMockInit.get();

    std::shared_ptr<server::application::SendRegistrationVerificationEmailCommandHandlerMock>
        sendRegistrationVerificationEmailCommandHandlerMock =
            std::make_shared<StrictMock<server::application::SendRegistrationVerificationEmailCommandHandlerMock>>();

    RegisterMessageHandler registerMessageHandler{std::move(registerUserCommandHandlerMockInit),
                                                  sendRegistrationVerificationEmailCommandHandlerMock};
};

TEST_F(RegisterMessageHandlerTest, handleValidRegisterUserMessage)
{
    EXPECT_CALL(*registerUserCommandHandlerMock,
                execute(server::application::RegisterUserCommandHandlerPayload{userEmail, userPassword}))
        .WillOnce(Return(server::application::RegisterUserCommandHandlerResult{
            {userId, userEmail, userPassword, userNickname, userIsActive, userEmailVerified, "123", "", ""}}));

    EXPECT_CALL(*sendRegistrationVerificationEmailCommandHandlerMock,
                execute(server::application::SendRegistrationVerificationEmailCommandHandlerPayload{userEmail}));

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
    EXPECT_CALL(*registerUserCommandHandlerMock,
                execute(server::application::RegisterUserCommandHandlerPayload{userEmail, userPassword}))
        .WillOnce(Throw(registerUserError));

    auto responseMessage = registerMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, registerUserErrorMessageResponse);
}
