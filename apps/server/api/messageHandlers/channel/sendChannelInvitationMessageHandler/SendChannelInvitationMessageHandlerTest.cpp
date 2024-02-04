#include "SendChannelInvitationMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerMock.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
auto friendId = "friendId";
auto channelId = "channelId";
auto friendEmail = "friend_email";
auto friendUser = server::domain::User{friendId, friendEmail, "", "", true, true, "123", "123", "", "", ""};
auto validPayloadJson = nlohmann::json{{"data", {{"email", friendEmail}, {"channelId", channelId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::SendChannelInvitation, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::SendChannelInvitationResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::SendChannelInvitationResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error findFriendEmailError("findFriendEmailError");
auto findFriendEmailErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendChannelInvitationResponse,
                              common::bytes::Bytes{R"({"error":"findFriendEmailError"})"}};

std::runtime_error sendFriendInvitationError("sendFriendInvitationError");
auto sendFriendInvitationErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendChannelInvitationResponse,
                              common::bytes::Bytes{R"({"error":"sendFriendInvitationError"})"}};
}

class SendChannelInvitationMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::shared_ptr<server::application::FindUserByEmailQueryHandlerMock> findUserByEmailQueryHandler =
        std::make_shared<StrictMock<server::application::FindUserByEmailQueryHandlerMock>>();

    std::unique_ptr<server::application::CreateChannelInvitationCommandHandlerMock>
        createChannelInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateChannelInvitationCommandHandlerMock>>();
    server::application::CreateChannelInvitationCommandHandlerMock* createChannelInvitationCommandHandlerMock =
        createChannelInvitationCommandHandlerMockInit.get();

    SendChannelInvitationMessageHandler sendChannelInvitationMessageHandler{
        tokenServiceMock, findUserByEmailQueryHandler, std::move(createChannelInvitationCommandHandlerMockInit)};
};

TEST_F(SendChannelInvitationMessageHandlerTest, handleValidFriendInvitationMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{friendUser}));
    EXPECT_CALL(
        *createChannelInvitationCommandHandlerMock,
        execute(server::application::CreateChannelInvitationCommandHandlerPayload{userId, friendId, channelId}));

    auto responseMessage = sendChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(SendChannelInvitationMessageHandlerTest, handleFriendInvitationMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = sendChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(SendChannelInvitationMessageHandlerTest, handleFriendInvitationMessageWithErrorWhileFindingFriendEmail)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Throw(findFriendEmailError));

    auto responseMessage = sendChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, findFriendEmailErrorMessageResponse);
}

TEST_F(SendChannelInvitationMessageHandlerTest, handleFriendInvitationMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{friendUser}));
    EXPECT_CALL(*createChannelInvitationCommandHandlerMock,
                execute(server::application::CreateChannelInvitationCommandHandlerPayload{userId, friendId, channelId}))
        .WillOnce(Throw(sendFriendInvitationError));

    auto responseMessage = sendChannelInvitationMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, sendFriendInvitationErrorMessageResponse);
}
