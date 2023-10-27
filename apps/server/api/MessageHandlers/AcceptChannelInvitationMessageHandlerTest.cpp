#include "AcceptChannelInvitationMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

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
auto message = common::messages::Message{common::messages::MessageId::AcceptChannelInvitation, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeChannelInvitationResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::ChangeChannelInvitationResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error acceptInvitationToChannelError("acceptInvitationToChannelError");
auto createChannelErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeChannelInvitationResponse,
                              common::bytes::Bytes{R"({"error":"acceptInvitationToChannelError"})"}};
}

class AcceptChannelInvitationMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::AcceptChannelInvitationCommandHandlerMock>
        acceptChannelInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::AcceptChannelInvitationCommandHandlerMock>>();
    server::application::AcceptChannelInvitationCommandHandlerMock* acceptChannelInvitationCommandHandlerMock =
        acceptChannelInvitationCommandHandlerMockInit.get();

    AcceptChannelInvitationMessageHandler acceptChannelInvitationMessageHandler{
        tokenServiceMock, std::move(acceptChannelInvitationCommandHandlerMockInit)};
};

TEST_F(AcceptChannelInvitationMessageHandlerTest, handleValidCreateChannelMessage)
{

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(
        *acceptChannelInvitationCommandHandlerMock,
        execute(server::application::AcceptChannelInvitationCommandHandlerPayload{recipientId, channelInvitationId}));

    auto responseMessage = acceptChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(AcceptChannelInvitationMessageHandlerTest, handleCreateChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = acceptChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(AcceptChannelInvitationMessageHandlerTest, handleCreateChannelMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(
        *acceptChannelInvitationCommandHandlerMock,
        execute(server::application::AcceptChannelInvitationCommandHandlerPayload{recipientId, channelInvitationId}))
        .WillOnce(Throw(acceptInvitationToChannelError));

    auto responseMessage = acceptChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, createChannelErrorMessageResponse);
}
