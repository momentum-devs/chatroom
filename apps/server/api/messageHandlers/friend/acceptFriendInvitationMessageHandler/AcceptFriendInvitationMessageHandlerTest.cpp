#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "AcceptFriendInvitationMessageHandler.h"
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto requestId = "requestId";
auto token = "token";
auto recipientId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{recipientId};
auto validPayloadJson = nlohmann::json{{"data", nlohmann::json{{"requestId", requestId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::AcceptFriendInvitations, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendInvitationsResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::ChangeFriendInvitationsResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error acceptFriendInvitationError("acceptFriendInvitationError");
auto acceptFriendInvitationErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeFriendInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"acceptFriendInvitationError"})"}};
}

class AcceptFriendInvitationMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::AcceptFriendInvitationCommandHandlerMock>
        acceptFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::AcceptFriendInvitationCommandHandlerMock>>();
    server::application::AcceptFriendInvitationCommandHandlerMock* acceptFriendInvitationCommandHandlerMock =
        acceptFriendInvitationCommandHandlerMockInit.get();

    AcceptFriendInvitationMessageHandler acceptFriendInvitationMessageHandler{
        tokenServiceMock, std::move(acceptFriendInvitationCommandHandlerMockInit)};
};

TEST_F(AcceptFriendInvitationMessageHandlerTest, handleValidAcceptFriendInvitationMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*acceptFriendInvitationCommandHandlerMock,
                execute(server::application::AcceptFriendInvitationCommandHandlerPayload{recipientId, requestId}));

    auto responseMessage = acceptFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(AcceptFriendInvitationMessageHandlerTest, handleAcceptFriendInvitationMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = acceptFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(AcceptFriendInvitationMessageHandlerTest, handleAcceptFriendInvitationMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*acceptFriendInvitationCommandHandlerMock,
                execute(server::application::AcceptFriendInvitationCommandHandlerPayload{recipientId, requestId}))
        .WillOnce(Throw(acceptFriendInvitationError));

    auto responseMessage = acceptFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, acceptFriendInvitationErrorMessageResponse);
}
