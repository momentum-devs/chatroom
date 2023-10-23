#include "GetUserChannelsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerMock.h"
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
auto message = common::messages::Message{common::messages::MessageId::GetUserChannels, validPayload};

auto noFriendRequestResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendRequestMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                              common::bytes::Bytes{noFriendRequestResponsePayloadJson.dump()}};

auto requestId1 = "id1";
auto friendName1 = "id1";
auto channelId2 = "id1";
auto channelName2 = "id1";
auto fewChannelResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", requestId1}, {"name", friendName1}, {"isOwner", true}},
                                                   {{"id", channelId2}, {"name", channelName2}, {"isOwner", true}}})}};

auto fewChannelMessageResponse = common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                                           common::bytes::Bytes{fewChannelResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserChannelsError("getUserChannelsError");
auto getUserChannelsErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::GetUserChannelsResponse, common::bytes::Bytes{R"({"error":"getUserChannelsError"})"}};
}

class GetUserChannelsMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandlerMock>
        findChannelsToWhichUserBelongsQueryHandlerMockInit =
            std::make_unique<StrictMock<server::application::FindChannelsToWhichUserBelongsQueryHandlerMock>>();
    server::application::FindChannelsToWhichUserBelongsQueryHandlerMock*
        findChannelsToWhichUserBelongsQueryHandlerMock = findChannelsToWhichUserBelongsQueryHandlerMockInit.get();

    GetUserChannelsMessageHandler getUserChannelsMessageHandler{
        tokenServiceMock, std::move(findChannelsToWhichUserBelongsQueryHandlerMockInit)};
};

TEST_F(GetUserChannelsMessageHandlerTest, handleValidGetUserChannelsMessageWithNoChannels)
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

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{}));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendRequestMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleValidGetUserChannelsMessageWithFewChannels)
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

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{
            {{requestId1, friendName1, user, "", ""}, {channelId2, channelName2, user, "", ""}}}));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewChannelMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleGetUserChannelsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleGetUserChannelsMessageWithErrorWhileHandling)
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

    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
