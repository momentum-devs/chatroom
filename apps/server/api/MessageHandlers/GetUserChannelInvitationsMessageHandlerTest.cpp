#include "GetUserChannelInvitationsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
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

auto noChannelResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noChannelMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                              common::bytes::Bytes{noChannelResponsePayloadJson.dump()}};

auto channelId1 = "id1";
auto channelName1 = "id1";
auto channelId2 = "channelId2";
auto channelName2 = "channelName2";
auto fewChannelResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", channelId1}, {"name", channelName1}},
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

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedChannelInvitationsQueryHandlerResult{}));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noChannelMessageResponse);
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

    const auto user = std::make_shared<server::domain::User>(userId, email, password, nickname, active, emailVerified,
                                                             verificationCode, createdAt, updatedAt);

    const auto channel1 =
        std::make_shared<server::domain::Channel>(channelId1, channelName1, user, createdAt, updatedAt);
    const auto channel2 =
        std::make_shared<server::domain::Channel>(channelId2, channelName2, user, createdAt, updatedAt);

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedChannelInvitationsQueryHandlerResult{
            {{channelId1, user, user, channel1, "", ""}, {channelId2, user, user, channel2, "", ""}}}));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewChannelMessageResponse);
}

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleGetUserChannelsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserChannelInvitationsMessageHandlerTest, handleGetUserChannelsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
