#include "LeaveChannelMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
auto channelId = "channelId";
auto validPayloadJson = nlohmann::json{{"data", {{"channelId", channelId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::SendFriendRequest, validPayload};
auto validMessageResponse =
    common::messages::Message{common::messages::MessageId::LeftTheChannelResponse, common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::LeftTheChannelResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error sendFriendRequestError("sendFriendRequestError");
auto sendFriendRequestErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::LeftTheChannelResponse, common::bytes::Bytes{R"({"error":"sendFriendRequestError"})"}};
}

class LeaveChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::RemoveUserFromChannelCommandHandlerMock>
        removeUserFromChannelCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::RemoveUserFromChannelCommandHandlerMock>>();
    server::application::RemoveUserFromChannelCommandHandlerMock* removeUserFromChannelCommandHandlerMock =
        removeUserFromChannelCommandHandlerMockInit.get();

    LeaveChannelMessageHandler leaveChannelMessageHandler{tokenServiceMock,
                                                          std::move(removeUserFromChannelCommandHandlerMockInit)};
};

TEST_F(LeaveChannelMessageHandlerTest, handleValidFriendRequestMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*removeUserFromChannelCommandHandlerMock,
                execute(server::application::RemoveUserFromChannelCommandHandlerPayload{userId, channelId}));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(LeaveChannelMessageHandlerTest, handleFriendRequestMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(LeaveChannelMessageHandlerTest, handleFriendRequestMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*removeUserFromChannelCommandHandlerMock,
                execute(server::application::RemoveUserFromChannelCommandHandlerPayload{userId, channelId}))
        .WillOnce(Throw(sendFriendRequestError));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, sendFriendRequestErrorMessageResponse);
}
