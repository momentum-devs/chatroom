#include "GetUserDataMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
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
    server::tests::UserTestFactory userTestFactory;

    std::string token = "token";
    std::shared_ptr<server::domain::User> user = userTestFactory.createDomainUser();
    const server::application::VerifyTokenResult verifyTokenResult{user->getId()};

    nlohmann::json validPayloadJson{{"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message message{common::messages::MessageId::GetUserData, validPayload};

    nlohmann::json validMessageResponsePayloadJson{
        {"data",
         {{"email", user->getEmail()}, {"nickname", user->getNickname()}, {"verified", user->isEmailVerified()}}}};
    common::messages::Message validMessageResponse{common::messages::MessageId::GetUserDataResponse,
                                                   common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

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
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindUserQueryHandlerResult{*user}));

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
    EXPECT_CALL(*findUserQueryHandlerMock, execute(server::application::FindUserQueryHandlerPayload{user->getId()}))
        .WillOnce(Throw(getUserDataError));

    auto responseMessage = getUserDataMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserDataErrorMessageResponse);
}
