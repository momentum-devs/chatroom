#include "GetUserChannelsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserChannels, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

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
    server::tests::UserTestFactory userTestFactory;

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
    const auto user = userTestFactory.createDomainUser();

    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{}));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleValidGetUserChannelsMessageWithFewChannels)
{
    const auto user = userTestFactory.createDomainUser();

    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindChannelsToWhichUserBelongsQueryHandlerResult{
            {{requestId1, friendName1, user, "", "", ""}, {channelId2, channelName2, user, "", "", ""}}}));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, fewChannelMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleGetUserChannelsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserChannelsMessageHandlerTest, handleGetUserChannelsMessageWithErrorWhileHandling)
{
    const auto user = userTestFactory.createDomainUser();

    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelsToWhichUserBelongsQueryHandlerMock,
                execute(server::application::FindChannelsToWhichUserBelongsQueryHandlerPayload{user->getId()}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserChannelsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
