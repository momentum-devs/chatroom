#include "GetUserFriendRequestsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserChannelInvitations, validPayload};

auto noFriendRequestResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendRequestMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendRequestsResponse,
                              common::bytes::Bytes{noFriendRequestResponsePayloadJson.dump()}};

auto requestId1 = "id1";
auto friendName1 = "friendName1";
auto requestId2 = "id2";
auto friendName2 = "friendName2";
auto fewFriendRequestsResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", requestId1}, {"name", friendName1}},
                                                   {{"id", requestId2}, {"name", friendName2}}})}};

auto fewFriendRequestsMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendRequestsResponse,
                              common::bytes::Bytes{fewFriendRequestsResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetFriendRequestsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserChannelsError("getUserChannelsError");
auto getUserChannelsErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendRequestsResponse,
                              common::bytes::Bytes{R"({"error":"getUserChannelsError"})"}};
}

class GetUserFriendRequestsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindReceivedFriendInvitationsQueryHandlerMock>
        findReceivedFriendInvitationsQueryHandlerMockInit =
            std::make_unique<StrictMock<server::application::FindReceivedFriendInvitationsQueryHandlerMock>>();
    server::application::FindReceivedFriendInvitationsQueryHandlerMock* findReceivedFriendInvitationsQueryHandlerMock =
        findReceivedFriendInvitationsQueryHandlerMockInit.get();

    GetUserFriendRequestsMessageHandler getUserFriendRequestsMessageHandler{
        tokenServiceMock, std::move(findReceivedFriendInvitationsQueryHandlerMockInit)};
};

TEST_F(GetUserFriendRequestsMessageHandlerTest, handleValidGetUserFriendRequestsMessageWithNoChannels)
{
    const auto userId = faker::String::uuid();

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedFriendInvitationsQueryHandlerResult{}));

    auto responseMessage = getUserFriendRequestsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendRequestMessageResponse);
}

TEST_F(GetUserFriendRequestsMessageHandlerTest, handleValidGetUserFriendRequestsMessageWithFewChannels)
{
    const auto userId = faker::String::uuid();
    const auto email = "email";
    const auto password = "password";
    const auto active = true;
    const auto emailVerified = true;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();

    const auto user1 = std::make_shared<server::domain::User>(userId, email, password, friendName1, active,
                                                              emailVerified, verificationCode, createdAt, updatedAt);
    const auto user2 = std::make_shared<server::domain::User>(userId, email, password, friendName2, active,
                                                              emailVerified, verificationCode, createdAt, updatedAt);

    const auto friendRequest1 =
        std::make_shared<server::domain::FriendInvitation>(requestId1, user1, user2, createdAt, updatedAt);
    const auto friendRequest2 =
        std::make_shared<server::domain::FriendInvitation>(requestId2, user2, user2, createdAt, updatedAt);

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(
            server::application::FindReceivedFriendInvitationsQueryHandlerResult{{*friendRequest1, *friendRequest2}}));

    auto responseMessage = getUserFriendRequestsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewFriendRequestsMessageResponse);
}

TEST_F(GetUserFriendRequestsMessageHandlerTest, handleGetUserFriendRequestsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserFriendRequestsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserFriendRequestsMessageHandlerTest, handleGetUserFriendRequestsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserFriendRequestsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
