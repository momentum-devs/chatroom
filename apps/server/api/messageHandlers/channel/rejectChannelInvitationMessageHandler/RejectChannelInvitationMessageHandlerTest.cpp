#include "RejectChannelInvitationMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include <fmt/format.h>
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto channelInvitationId = "channelInvitationId";
auto token = "token";
auto recipientId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{recipientId};
auto validPayloadJson = nlohmann::json{{"data", nlohmann::json{{"channelId", channelInvitationId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::RejectChannelInvitation, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeChannelInvitationResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::ChangeChannelInvitationResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error acceptInvitationToChannelError("rejectInvitationToChannelError");
auto createChannelErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeChannelInvitationResponse,
                              common::bytes::Bytes{R"({"error":"rejectInvitationToChannelError"})"}};
}

class RejectChannelInvitationMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::RejectChannelInvitationCommandHandlerMock>
        rejectChannelInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::RejectChannelInvitationCommandHandlerMock>>();
    server::application::RejectChannelInvitationCommandHandlerMock* rejectChannelInvitationCommandHandlerMock =
        rejectChannelInvitationCommandHandlerMockInit.get();

    RejectChannelInvitationMessageHandler rejectChannelInvitationMessageHandler{
        tokenServiceMock, std::move(rejectChannelInvitationCommandHandlerMockInit)};
};

TEST_F(RejectChannelInvitationMessageHandlerTest, handleValidCreateChannelMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(
        *rejectChannelInvitationCommandHandlerMock,
        execute(server::application::RejectChannelInvitationCommandHandlerPayload{recipientId, channelInvitationId}));

    auto responseMessage = rejectChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(RejectChannelInvitationMessageHandlerTest, handleCreateChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = rejectChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(RejectChannelInvitationMessageHandlerTest, handleCreateChannelMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(
        *rejectChannelInvitationCommandHandlerMock,
        execute(server::application::RejectChannelInvitationCommandHandlerPayload{recipientId, channelInvitationId}))
        .WillOnce(Throw(acceptInvitationToChannelError));

    auto responseMessage = rejectChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, createChannelErrorMessageResponse);
}
