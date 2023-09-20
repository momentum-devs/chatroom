#include "VerifyUserMessageHandle.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerMock.h"
#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
auto userEmail = "userEmail";
auto validVerificationCode = "validVerificationCode";

auto validPayloadJson = nlohmann::json{{"data", {{"verificationCode", validVerificationCode}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::VerifyUser, validPayload};

auto validMessageResponsePayloadJson = nlohmann::json{"ok"};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::VerifyUserResponse,
                                                      common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::VerifyUserResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error userNotFoundError("userNotFound");
auto userNotFoundErrorMessageResponse = common::messages::Message{common::messages::MessageId::VerifyUserResponse,
                                                                  common::bytes::Bytes{R"({"error":"userNotFound"})"}};

auto invalidVerificationCode = "invalidVerificationCode";
auto invalidPayloadJson = nlohmann::json{{"data", {{"verificationCode", invalidVerificationCode}}}, {"token", token}};
auto invalidPayload = common::bytes::Bytes{invalidPayloadJson.dump()};
auto invalidMessage = common::messages::Message{common::messages::MessageId::VerifyUser, invalidPayload};

auto invalidVerificationCodeMessageResponse = common::messages::Message{
    common::messages::MessageId::VerifyUserResponse, common::bytes::Bytes{R"({"error":"invalid verification code"})"}};
}

class VerifyUserMessageHandleTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::VerifyUserEmailCommandHandlerMock> verifyUserEmailCommandHandlerInit =
        std::make_unique<StrictMock<server::application::VerifyUserEmailCommandHandlerMock>>();
    server::application::VerifyUserEmailCommandHandlerMock* verifyUserEmailCommandHandler =
        verifyUserEmailCommandHandlerInit.get();

    std::shared_ptr<server::application::FindUserQueryHandlerMock> findUserQueryHandlerMock =
        std::make_shared<StrictMock<server::application::FindUserQueryHandlerMock>>();

    VerifyUserMessageHandle verifyUserMessageHandle{tokenServiceMock, std::move(verifyUserEmailCommandHandlerInit),
                                                    findUserQueryHandlerMock};
};

TEST_F(VerifyUserMessageHandleTest, handleValidUserVerifyRequest)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{userId}))
        .WillOnce(Return(
            server::application::FindUserQueryHandlerResult{{userId, userEmail, "", "", false, false, "123", "", ""}}));
    EXPECT_CALL(*verifyUserEmailCommandHandler,
                execute(server::application::VerifyUserEmailCommandHandlerPayload{userEmail, validVerificationCode}))
        .WillOnce(Return(server::application::VerifyUserEmailCommandHandlerResult{true}));

    auto responseMessage = verifyUserMessageHandle.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(VerifyUserMessageHandleTest, handleUserVerifyRequestWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = verifyUserMessageHandle.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(VerifyUserMessageHandleTest, handleUserVerifyRequestForNotExistingUser)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{userId}))
        .WillOnce(Throw(userNotFoundError));

    auto responseMessage = verifyUserMessageHandle.handleMessage(message);

    EXPECT_EQ(responseMessage, userNotFoundErrorMessageResponse);
}

TEST_F(VerifyUserMessageHandleTest, handleUserVerifyRequestWithInvalidCode)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{userId}))
        .WillOnce(Return(
            server::application::FindUserQueryHandlerResult{{userId, userEmail, "", "", false, false, "123", "", ""}}));
    EXPECT_CALL(*verifyUserEmailCommandHandler,
                execute(server::application::VerifyUserEmailCommandHandlerPayload{userEmail, invalidVerificationCode}))
        .WillOnce(Return(server::application::VerifyUserEmailCommandHandlerResult{false}));

    auto responseMessage = verifyUserMessageHandle.handleMessage(invalidMessage);

    EXPECT_EQ(responseMessage, invalidVerificationCodeMessageResponse);
}
