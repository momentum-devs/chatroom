#include "RemoveFromFriendsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
auto userFriendId = "userFriendId";
auto validPayloadJson = nlohmann::json{{"data", {{"userFriendId", userFriendId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::RemoveFromFriends, validPayload};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::RemoveFromFriendsResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::RemoveFromFriendsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error removeFromFriendsError("removeFromFriendsError");
auto deleteUserErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::RemoveFromFriendsResponse,
                              common::bytes::Bytes{R"({"error":"removeFromFriendsError"})"}};
}

class RemoveFromFriendsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::DeleteFriendshipCommandHandlerMock> deleteFriendshipCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::DeleteFriendshipCommandHandlerMock>>();
    server::application::DeleteFriendshipCommandHandlerMock* deleteFriendshipCommandHandlerMock =
        deleteFriendshipCommandHandlerMockInit.get();

    RemoveFromFriendsMessageHandler removeFromFriendsMessageHandler{tokenServiceMock,
                                                                    std::move(deleteFriendshipCommandHandlerMockInit)};
};

TEST_F(RemoveFromFriendsMessageHandlerTest, handleValidRemoveFromFriendsMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteFriendshipCommandHandlerMock,
                execute(server::application::DeleteFriendshipCommandHandlerPayload{userId, userFriendId}));

    auto responseMessage = removeFromFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(RemoveFromFriendsMessageHandlerTest, handleRemoveFromFriendsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = removeFromFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(RemoveFromFriendsMessageHandlerTest, handleRemoveFromFriendsMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteFriendshipCommandHandlerMock,
                execute(server::application::DeleteFriendshipCommandHandlerPayload{userId, userFriendId}))
        .WillOnce(Throw(removeFromFriendsError));

    auto responseMessage = removeFromFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, deleteUserErrorMessageResponse);
}
