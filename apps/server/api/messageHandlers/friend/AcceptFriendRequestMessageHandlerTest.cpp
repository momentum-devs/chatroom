#include "AcceptFriendRequestMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerMock.h"
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
auto message = common::messages::Message{common::messages::MessageId::AcceptFriendRequests, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error acceptFriendRequestError("acceptFriendRequestError");
auto acceptFriendRequestErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::ChangeFriendRequestsResponse,
                              common::bytes::Bytes{R"({"error":"acceptFriendRequestError"})"}};
}

class AcceptFriendRequestMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::AcceptFriendInvitationCommandHandlerMock>
        acceptFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::AcceptFriendInvitationCommandHandlerMock>>();
    server::application::AcceptFriendInvitationCommandHandlerMock* acceptFriendInvitationCommandHandlerMock =
        acceptFriendInvitationCommandHandlerMockInit.get();

    AcceptFriendRequestMessageHandler acceptFriendRequestMessageHandler{
        tokenServiceMock, std::move(acceptFriendInvitationCommandHandlerMockInit)};
};

TEST_F(AcceptFriendRequestMessageHandlerTest, handleValidAcceptFriendRequestMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*acceptFriendInvitationCommandHandlerMock,
                execute(server::application::AcceptFriendInvitationCommandHandlerPayload{recipientId, requestId}));

    auto responseMessage = acceptFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(AcceptFriendRequestMessageHandlerTest, handleAcceptFriendRequestMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = acceptFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(AcceptFriendRequestMessageHandlerTest, handleAcceptFriendRequestMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*acceptFriendInvitationCommandHandlerMock,
                execute(server::application::AcceptFriendInvitationCommandHandlerPayload{recipientId, requestId}))
        .WillOnce(Throw(acceptFriendRequestError));

    auto responseMessage = acceptFriendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, acceptFriendRequestErrorMessageResponse);
}
