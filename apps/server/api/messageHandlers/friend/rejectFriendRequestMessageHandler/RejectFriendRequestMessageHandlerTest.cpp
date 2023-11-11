#include "RejectFriendRequestMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

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
auto message = common::messages::Message{common::messages::MessageId::RejectFriendRequests, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error rejectFriendRequestError("rejectFriendRequestError");
auto rejectFriendRequestErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                              common::bytes::Bytes{R"({"error":"rejectFriendRequestError"})"}};
}

class RejectFriendRequestMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::RejectFriendInvitationCommandHandlerMock>
        rejectFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::RejectFriendInvitationCommandHandlerMock>>();
    server::application::RejectFriendInvitationCommandHandlerMock* rejectFriendInvitationCommandHandlerMock =
        rejectFriendInvitationCommandHandlerMockInit.get();

    RejectFriendRequestMessageHandler rejectFriendRequestMessageHandler{
        tokenServiceMock, std::move(rejectFriendInvitationCommandHandlerMockInit)};
};

TEST_F(RejectFriendRequestMessageHandlerTest, handleValidRejectFriendRequestMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*rejectFriendInvitationCommandHandlerMock,
                execute(server::application::RejectFriendInvitationCommandHandlerPayload{recipientId, requestId}));

    auto responseMessage = rejectFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(RejectFriendRequestMessageHandlerTest, handleRejectFriendRequestMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = rejectFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(RejectFriendRequestMessageHandlerTest, handleRejectFriendRequestMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*rejectFriendInvitationCommandHandlerMock,
                execute(server::application::RejectFriendInvitationCommandHandlerPayload{recipientId, requestId}))
        .WillOnce(Throw(rejectFriendRequestError));

    auto responseMessage = rejectFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, rejectFriendRequestErrorMessageResponse);
}
