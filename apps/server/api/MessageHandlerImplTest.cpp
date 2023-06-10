#include "MessageHandlerImpl.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
common::bytes::Bytes token{};
const auto id = 0;
const std::string validEmail = "abc@abc.com";
const std::string validPassword = "password";
const server::application::RegisterUserCommandHandlerPayload validRegisterUserCommandHandlerPayload{validEmail,
                                                                                                    validPassword};
const server::application::RegisterUserCommandHandlerResult validRegisterUserCommandHandlerResult{
    {id, validEmail, validPassword, validPassword}};
common::bytes::Bytes validRegisterPayload{
    std::format(R"({{"email":"{}","password":"{}"}})", validEmail, validPassword)};
common::messages::Message validRegisterMessage{common::messages::MessageId::Register, token, validRegisterPayload};
common::bytes::Bytes okResponse{R"({"ok"})"};

const std::string invalidEmail = "invalidEmail";
const std::string invalidPassword = "invalidPassword";
common::bytes::Bytes registerPayloadWithInvalidEmail{
    std::format(R"({{"email":"{}","password":"{}"}})", invalidEmail, invalidPassword)};
common::messages::Message registerMessagedWithInvalidEmail{common::messages::MessageId::Register, token,
                                                           registerPayloadWithInvalidEmail};
common::bytes::Bytes wrongEmailResponse{nlohmann::json{{"error", "wrong email address"}}.dump()};
}

class MessageHandlerImplTest : public Test
{
public:
    std::unique_ptr<server::application::RegisterUserCommandHandlerMock> registerUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::RegisterUserCommandHandlerMock>>();
    server::application::RegisterUserCommandHandlerMock* registerUserCommandHandlerMock =
        registerUserCommandHandlerMockInit.get();

    MessageHandlerImpl messageHandler{std::move(registerUserCommandHandlerMockInit)};
};

TEST_F(MessageHandlerImplTest, handleRegisterValidMessage)
{
    EXPECT_CALL(*registerUserCommandHandlerMock, execute(validRegisterUserCommandHandlerPayload))
        .WillOnce(Return(validRegisterUserCommandHandlerResult));

    auto response = messageHandler.handleMessage(validRegisterMessage);

    EXPECT_EQ(response.payload, okResponse);
}

TEST_F(MessageHandlerImplTest, handleRegisterMessageWithInvalidEmail)
{
    auto response = messageHandler.handleMessage(registerMessagedWithInvalidEmail);

    EXPECT_EQ(response.payload, wrongEmailResponse);
}