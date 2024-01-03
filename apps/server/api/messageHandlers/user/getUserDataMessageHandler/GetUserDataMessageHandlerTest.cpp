#include "GetUserDataMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include <fmt/format.h>
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserData, validPayload};
auto userEmail = "userEmail";
auto userPassword = "userPassword";
auto userNickname = "userNickname";
auto userIsActive = true;
auto userEmailVerified = true;

auto validMessageResponsePayloadJson =
    nlohmann::json{{"data", {{"email", userEmail}, {"nickname", userNickname}, {"verified", true}}}};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::GetUserDataResponse,
                                                      common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetUserDataResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserDataError("getUserDataError");
auto getUserDataErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::GetUserDataResponse, common::bytes::Bytes{R"({"error":"getUserDataError"})"}};
}

class GetUserDataMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindUserQueryHandlerMock> findUserQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindUserQueryHandlerMock>>();
    server::application::FindUserQueryHandlerMock* findUserQueryHandlerMock = findUserQueryHandlerMockInit.get();

    GetUserDataMessageHandler getUserDataMessageHandler{tokenServiceMock, std::move(findUserQueryHandlerMockInit)};
};

TEST_F(GetUserDataMessageHandlerTest, handleValidGetUserDataMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindUserQueryHandlerResult{
            {userId, userEmail, userPassword, userNickname, userIsActive, userEmailVerified, "123", "", "", ""}}));

    auto responseMessage = getUserDataMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(GetUserDataMessageHandlerTest, handleGetUserDataMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserDataMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserDataMessageHandlerTest, handleGetUserDataMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserDataError));

    auto responseMessage = getUserDataMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserDataErrorMessageResponse);
}
