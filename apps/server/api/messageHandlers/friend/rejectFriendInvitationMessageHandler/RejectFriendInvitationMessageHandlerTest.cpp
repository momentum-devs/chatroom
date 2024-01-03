#include "RejectFriendInvitationMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "fmt/format.h"
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
auto message = common::messages::Message{common::messages::MessageId::RejectFriendInvitations, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendInvitationsResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::ChangeFriendInvitationsResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error rejectFriendInvitationError("rejectFriendInvitationError");
auto rejectFriendInvitationErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeFriendInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"rejectFriendInvitationError"})"}};
}

class RejectFriendInvitationMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::RejectFriendInvitationCommandHandlerMock>
        rejectFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::RejectFriendInvitationCommandHandlerMock>>();
    server::application::RejectFriendInvitationCommandHandlerMock* rejectFriendInvitationCommandHandlerMock =
        rejectFriendInvitationCommandHandlerMockInit.get();

    RejectFriendInvitationMessageHandler rejectFriendInvitationMessageHandler{
        tokenServiceMock, std::move(rejectFriendInvitationCommandHandlerMockInit)};
};

TEST_F(RejectFriendInvitationMessageHandlerTest, handleValidRejectFriendInvitationMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*rejectFriendInvitationCommandHandlerMock,
                execute(server::application::RejectFriendInvitationCommandHandlerPayload{recipientId, requestId}));

    auto responseMessage = rejectFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(RejectFriendInvitationMessageHandlerTest, handleRejectFriendInvitationMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = rejectFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(RejectFriendInvitationMessageHandlerTest, handleRejectFriendInvitationMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*rejectFriendInvitationCommandHandlerMock,
                execute(server::application::RejectFriendInvitationCommandHandlerPayload{recipientId, requestId}))
        .WillOnce(Throw(rejectFriendInvitationError));

    auto responseMessage = rejectFriendInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, rejectFriendInvitationErrorMessageResponse);
}
