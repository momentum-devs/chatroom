#include "LogoutMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";

auto validPayloadJson = nlohmann::json{{"data", {}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::Logout, validPayload};
auto validMessageResponse = common::messages::Message{};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse =
    common::messages::Message{common::messages::MessageId::Error, common::bytes::Bytes{R"({"error":"invalidToken"})"}};
}

class LogoutMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::LogoutUserCommandHandlerMock> logoutUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::LogoutUserCommandHandlerMock>>();
    server::application::LogoutUserCommandHandlerMock* logoutUserCommandHandlerMock =
        logoutUserCommandHandlerMockInit.get();

    LogoutMessageHandler logoutMessageHandler{tokenServiceMock, std::move(logoutUserCommandHandlerMockInit)};
};

TEST_F(LogoutMessageHandlerTest, handleValidLogout)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*logoutUserCommandHandlerMock, execute(server::application::LogoutUserCommandHandlerPayload{userId}));

    auto responseMessage = logoutMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(LogoutMessageHandlerTest, handleLogoutMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = logoutMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}