#include "GetUserFriendInvitationsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/String.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetFriendInvitations, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

auto requestId1 = "id1";
auto friendName1 = "friendName1";
auto requestId2 = "id2";
auto friendName2 = "friendName2";
auto fewFriendInvitationsResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", requestId1}, {"name", friendName1}},
                                                   {{"id", requestId2}, {"name", friendName2}}})}};

auto fewFriendInvitationsMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                              common::bytes::Bytes{fewFriendInvitationsResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getFriendInvitationsError("getFriendInvitationsError");
auto getFriendInvitationsErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"getFriendInvitationsError"})"}};
}

class GetUserFriendInvitationsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindReceivedFriendInvitationsQueryHandlerMock>
        findReceivedFriendInvitationsQueryHandlerMockInit =
            std::make_unique<StrictMock<server::application::FindReceivedFriendInvitationsQueryHandlerMock>>();
    server::application::FindReceivedFriendInvitationsQueryHandlerMock* findReceivedFriendInvitationsQueryHandlerMock =
        findReceivedFriendInvitationsQueryHandlerMockInit.get();

    GetUserFriendInvitationsMessageHandler getUserFriendInvitationsMessageHandler{
        tokenServiceMock, std::move(findReceivedFriendInvitationsQueryHandlerMockInit)};
};

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleValidGetUserFriendInvitationsMessageWithNoFriendInvitations)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedFriendInvitationsQueryHandlerResult{}));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleValidGetUserFriendInvitationsMessageWithFewFriendInvitations)
{
    const auto userId = faker::String::uuid();
    const auto email = "email";
    const auto password = "password";
    const auto active = true;
    const auto emailVerified = true;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user1 = std::make_shared<server::domain::User>(
        userId, email, password, friendName1, active, emailVerified, verificationCode, createdAt, updatedAt, avatarUrl);
    const auto user2 = std::make_shared<server::domain::User>(
        userId, email, password, friendName2, active, emailVerified, verificationCode, createdAt, updatedAt, avatarUrl);

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    const auto friendInvitation1 =
        std::make_shared<server::domain::FriendInvitation>(requestId1, user1, user2, createdAt);
    const auto friendInvitation2 =
        std::make_shared<server::domain::FriendInvitation>(requestId2, user2, user2, createdAt);

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedFriendInvitationsQueryHandlerResult{
            {*friendInvitation1, *friendInvitation2}}));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewFriendInvitationsMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleGetUserFriendInvitationsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleGetUserFriendInvitationsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getFriendInvitationsError));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getFriendInvitationsErrorMessageResponse);
}
