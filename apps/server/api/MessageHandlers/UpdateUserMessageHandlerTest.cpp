#include "UpdateUserMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/updateUserCommandHandler/UpdateUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
auto userEmail = "userEmail";
auto userPassword = "userPassword";
auto userNickname = "userNickname";
auto userIsActive = true;
auto userEmailVerified = true;

auto validPayloadJson =
    nlohmann::json{{"data", {{"password", userPassword}, {"nickname", userNickname}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::UpdateUser, validPayload};

auto validMessageResponsePayloadJson = nlohmann::json{"ok"};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::UpdateUserResponse,
                                                      common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::UpdateUserResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error updateUserDataError("updateUserDataError");
auto updateUserDataErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::UpdateUserResponse, common::bytes::Bytes{R"({"error":"updateUserDataError"})"}};
}

class UpdateUserMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::UpdateUserCommandHandlerMock> updateUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::UpdateUserCommandHandlerMock>>();
    server::application::UpdateUserCommandHandlerMock* updateUserCommandHandlerMock =
        updateUserCommandHandlerMockInit.get();

    UpdateUserMessageHandler updateUserMessageHandler{tokenServiceMock, std::move(updateUserCommandHandlerMockInit)};
};

TEST_F(UpdateUserMessageHandlerTest, handleValidUpdateUserMessage)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*updateUserCommandHandlerMock,
                execute(server::application::UpdateUserCommandHandlerPayload{userId, userNickname, userPassword}))
        .WillOnce(Return(server::application::UpdateUserCommandHandlerResult{
            {userId, userEmail, userPassword, userNickname, userIsActive, userEmailVerified, "123", "", ""}}));

    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(UpdateUserMessageHandlerTest, handleUpdateUserMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(UpdateUserMessageHandlerTest, handleUpdateUserMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*updateUserCommandHandlerMock,
                execute(server::application::UpdateUserCommandHandlerPayload{userId, userNickname, userPassword}))
        .WillOnce(Throw(updateUserDataError));

    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, updateUserDataErrorMessageResponse);
}
