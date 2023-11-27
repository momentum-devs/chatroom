#include "LeaveChannelMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/leaveChannelCommandHandler/LeaveChannelCommandHandlerMock.h"
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
auto message = common::messages::Message{common::messages::MessageId::SendFriendInvitation, validPayload};
auto validMessageResponse =
    common::messages::Message{common::messages::MessageId::LeftTheChannelResponse, common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::LeftTheChannelResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error sendFriendInvitationError("sendFriendInvitationError");
auto sendFriendInvitationErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::LeftTheChannelResponse,
                              common::bytes::Bytes{R"({"error":"sendFriendInvitationError"})"}};
}

class LeaveChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::LeaveChannelCommandHandlerMock> leaveChannelCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::LeaveChannelCommandHandlerMock>>();

    server::application::LeaveChannelCommandHandlerMock* leaveChannelCommandHandlerMock =
        leaveChannelCommandHandlerMockInit.get();

    LeaveChannelMessageHandler leaveChannelMessageHandler{tokenServiceMock,
                                                          std::move(leaveChannelCommandHandlerMockInit)};
};

TEST_F(LeaveChannelMessageHandlerTest, handleValidFriendInvitationMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*leaveChannelCommandHandlerMock,
                execute(server::application::LeaveChannelCommandHandlerPayload{userId, channelId}));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(LeaveChannelMessageHandlerTest, handleFriendInvitationMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(LeaveChannelMessageHandlerTest, handleFriendInvitationMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*leaveChannelCommandHandlerMock,
                execute(server::application::LeaveChannelCommandHandlerPayload{userId, channelId}))
        .WillOnce(Throw(sendFriendInvitationError));

    auto responseMessage = leaveChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, sendFriendInvitationErrorMessageResponse);
}
