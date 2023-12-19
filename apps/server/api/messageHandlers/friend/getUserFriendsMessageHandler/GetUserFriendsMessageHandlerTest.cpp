#include "GetUserFriendsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findUserFriendsQueryHandler/FindUserFriendsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserFriends, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

auto friendId1 = "id1";
auto friendName1 = "friendName1";
auto friendId2 = "id2";
auto friendName2 = "friendName2";
auto fewFriendInvitationsResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", friendId1}, {"name", friendName1}, {"isActive", false}},
                                                   {{"id", friendId2}, {"name", friendName2}, {"isActive", true}}})}};

auto fewFriendInvitationsMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                              common::bytes::Bytes{fewFriendInvitationsResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserChannelsError("getUserChannelsError");
auto getUserChannelsErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::GetUserFriendsResponse, common::bytes::Bytes{R"({"error":"getUserChannelsError"})"}};
}

class GetUserFriendsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindUserFriendsQueryHandlerMock> findUserFriendsQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindUserFriendsQueryHandlerMock>>();
    server::application::FindUserFriendsQueryHandlerMock* findUserFriendsQueryHandlerMock =
        findUserFriendsQueryHandlerMockInit.get();

    GetUserFriendsMessageHandler getUserFriendsMessageHandler{tokenServiceMock,
                                                              std::move(findUserFriendsQueryHandlerMockInit)};
};

TEST_F(GetUserFriendsMessageHandlerTest, handleValidGetUserFriendsMessageWithNoFriends)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendsQueryHandlerMock,
                execute(server::application::FindUserFriendsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindUserFriendsQueryHandlerResult{}));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleValidGetUserFriendsMessageWithFewFriends)
{
    const auto userId = faker::String::uuid();
    const auto email = "email";
    const auto password = "password";
    const auto emailVerified = true;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user1 =
        std::make_shared<server::domain::User>(friendId1, email, password, friendName1, false, emailVerified,
                                               verificationCode, createdAt, updatedAt, avatarUrl);
    const auto user2 =
        std::make_shared<server::domain::User>(friendId2, email, password, friendName2, true, emailVerified,
                                               verificationCode, createdAt, updatedAt, avatarUrl);

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendsQueryHandlerMock,
                execute(server::application::FindUserFriendsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindUserFriendsQueryHandlerResult{{*user1, *user2}}));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewFriendInvitationsMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleGetUserFriendsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleGetUserFriendsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendsQueryHandlerMock,
                execute(server::application::FindUserFriendsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
