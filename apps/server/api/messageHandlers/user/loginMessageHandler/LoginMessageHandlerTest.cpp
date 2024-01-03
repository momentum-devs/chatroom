#include "LoginMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerMock.h"

#include "fmt/format.h"
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userEmail = "userEmail@email.com";
auto userPassword = "userPassword";
auto validPayloadJson = nlohmann::json{{"email", userEmail}, {"password", userPassword}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::Login, validPayload};
auto validMessageResponsePayloadJson = nlohmann::json{{"token", token}};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::LoginResponse,
                                                      common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

auto invalidUserEmail = "invalidUserEmail";
auto payloadJsonWithInvalidEmail = nlohmann::json{{"email", invalidUserEmail}, {"password", userPassword}};
auto payloadWithInvalidEmail = common::bytes::Bytes{payloadJsonWithInvalidEmail.dump()};
auto messageWithInvalidEmail = common::messages::Message{common::messages::MessageId::Login, payloadWithInvalidEmail};
auto invalidEmailMessageResponsePayloadJson = nlohmann::json{{"error", "wrong email address"}};
auto invalidEmailMessageResponse = common::messages::Message{
    common::messages::MessageId::LoginResponse, common::bytes::Bytes{invalidEmailMessageResponsePayloadJson.dump()}};

std::runtime_error loginUserError("loginUserError");
auto loginUserErrorMessageResponse = common::messages::Message{common::messages::MessageId::LoginResponse,
                                                               common::bytes::Bytes{R"({"error":"loginUserError"})"}};
}

class LoginMessageHandlerTest : public Test
{
public:
    std::unique_ptr<server::application::LoginUserCommandHandlerMock> loginUserCommandHandlerMockMockInit =
        std::make_unique<StrictMock<server::application::LoginUserCommandHandlerMock>>();
    server::application::LoginUserCommandHandlerMock* loginUserCommandHandlerMockMock =
        loginUserCommandHandlerMockMockInit.get();

    LoginMessageHandler loginMessageHandler{std::move(loginUserCommandHandlerMockMockInit)};
};

TEST_F(LoginMessageHandlerTest, handleValidLoginUserMessage)
{
    EXPECT_CALL(*loginUserCommandHandlerMockMock,
                execute(server::application::LoginUserCommandHandlerPayload{userEmail, userPassword}))
        .WillOnce(Return(server::application::LoginUserCommandHandlerResult{token}));

    auto responseMessage = loginMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(LoginMessageHandlerTest, handleLoginUserMessageWithInvalidEmail)
{
    auto responseMessage = loginMessageHandler.handleMessage(messageWithInvalidEmail);

    EXPECT_EQ(responseMessage, invalidEmailMessageResponse);
}

TEST_F(LoginMessageHandlerTest, handleLoginUserMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*loginUserCommandHandlerMockMock,
                execute(server::application::LoginUserCommandHandlerPayload{userEmail, userPassword}))
        .WillOnce(Throw(loginUserError));

    auto responseMessage = loginMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, loginUserErrorMessageResponse);
}
