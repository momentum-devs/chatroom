#include "GetUserChannelsMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserChannels, validPayload};

auto noChannelResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noChannelMessageResponse = common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                                          common::bytes::Bytes{noChannelResponsePayloadJson.dump()}};

auto channelId1 = "id1";
auto channelName1 = "id1";
auto channelId2 = "id1";
auto channelName2 = "id1";
auto fewChannelResponsePayloadJson =
    nlohmann::json{{"data", nlohmann::json::array({{{"id", channelId1}, {"name", channelName1}},
                                                   {{"id", channelId2}, {"name", channelName2}}})}};

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
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{}));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noChannelMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleValidGetUserChannelsMessageWithFewChannels)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{
            {{channelId1, channelName1, "", "", ""}, {channelId2, channelName2, "", "", ""}}}));

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
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
