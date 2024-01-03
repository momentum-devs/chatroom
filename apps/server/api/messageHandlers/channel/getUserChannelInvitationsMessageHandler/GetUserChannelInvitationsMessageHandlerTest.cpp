#include "GetUserChannelInvitationsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"
#include <fmt/format.h>
#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserChannelInvitations, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

auto requestId1 = "id1";
auto friendName1 = "id1";
auto channelId2 = "channelId2";
auto channelName2 = "channelName2";
auto fewChannelResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", requestId1}, {"name", friendName1}},
                                                   {{"id", channelId2}, {"name", channelName2}}})}};

auto fewChannelMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                              common::bytes::Bytes{fewChannelResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserChannelsError("getUserChannelsError");
auto getUserChannelsErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"getUserChannelsError"})"}};
}

class GetUserChannelInvitationsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindReceivedChannelInvitationsQueryHandlerMock>
        findReceivedChannelInvitationsQueryHandlerMockInit =
            std::make_unique<StrictMock<server::application::FindReceivedChannelInvitationsQueryHandlerMock>>();
    server::application::FindReceivedChannelInvitationsQueryHandlerMock*
        findReceivedChannelInvitationsQueryHandlerMock = findReceivedChannelInvitationsQueryHandlerMockInit.get();

    GetUserChannelInvitationsMessageHandler getUserChannelInvitationsMessageHandler{
        tokenServiceMock, std::move(findReceivedChannelInvitationsQueryHandlerMockInit)};
};

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleValidGetUserChannelsMessageWithNoChannels)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedChannelInvitationsQueryHandlerResult{}));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleValidGetUserChannelsMessageWithFewChannels)
{
    const auto userId = faker::String::uuid();
    const auto email = faker::Internet::email();
    const auto password = faker::Internet::password();
    const auto nickname = faker::Internet::username();
    const auto active = faker::Datatype::boolean();
    const auto emailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user = std::make_shared<server::domain::User>(userId, email, password, nickname, active, emailVerified,
                                                             verificationCode, createdAt, updatedAt, avatarUrl);

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    const auto channel1 =
        std::make_shared<server::domain::Channel>(requestId1, friendName1, user, createdAt, updatedAt, avatarUrl);
    const auto channel2 =
        std::make_shared<server::domain::Channel>(channelId2, channelName2, user, createdAt, updatedAt, avatarUrl);

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedChannelInvitationsQueryHandlerResult{
            {{requestId1, user, user, channel1, ""}, {channelId2, user, user, channel2, ""}}}));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewChannelMessageResponse);
}

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleGetUserChannelsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleGetUserChannelsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
