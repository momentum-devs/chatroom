#include "FriendRequestMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerMock.h"
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
auto friendEmail = "friend_email";
auto friendUser = server::domain::User{friendId, friendEmail, "", "", true, true, "123", "", ""};
auto validPayloadJson = nlohmann::json{{"data", {{"friend_email", friendEmail}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::SendFriendRequest, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::SendFriendRequestResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::SendFriendRequestResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error findFriendEmailError("findFriendEmailError");
auto findFriendEmailErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendFriendRequestResponse,
                              common::bytes::Bytes{R"({"error":"findFriendEmailError"})"}};

std::runtime_error sendFriendRequestError("sendFriendRequestError");
auto sendFriendRequestErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendFriendRequestResponse,
                              common::bytes::Bytes{R"({"error":"sendFriendRequestError"})"}};
}

class FriendRequestMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::shared_ptr<server::application::FindUserByEmailQueryHandlerMock> findUserByEmailQueryHandler =
        std::make_shared<StrictMock<server::application::FindUserByEmailQueryHandlerMock>>();

    std::unique_ptr<server::application::CreateFriendInvitationCommandHandlerMock>
        createFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateFriendInvitationCommandHandlerMock>>();
    server::application::CreateFriendInvitationCommandHandlerMock* createFriendInvitationCommandHandlerMock =
        createFriendInvitationCommandHandlerMockInit.get();

    FriendRequestMessageHandler friendRequestMessageHandler{tokenServiceMock, findUserByEmailQueryHandler,
                                                            std::move(createFriendInvitationCommandHandlerMockInit)};
};

TEST_F(FriendRequestMessageHandlerTest, handleValidFriendRequestMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{friendUser}));
    EXPECT_CALL(*createFriendInvitationCommandHandlerMock,
                execute(server::application::CreateFriendInvitationCommandHandlerPayload{userId, friendId}));

    auto responseMessage = friendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(FriendRequestMessageHandlerTest, handleFriendRequestMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = friendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(FriendRequestMessageHandlerTest, handleFriendRequestMessageWithErrorWhileFindingFriendEmail)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Throw(findFriendEmailError));

    auto responseMessage = friendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, findFriendEmailErrorMessageResponse);
}

TEST_F(FriendRequestMessageHandlerTest, handleFriendRequestMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{friendUser}));
    EXPECT_CALL(*createFriendInvitationCommandHandlerMock,
                execute(server::application::CreateFriendInvitationCommandHandlerPayload{userId, friendId}))
        .WillOnce(Throw(sendFriendRequestError));

    auto responseMessage = friendRequestMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, sendFriendRequestErrorMessageResponse);
}
