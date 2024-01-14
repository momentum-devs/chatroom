#include "GetUserChannelInvitationsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/channelInvitationTestFactory/ChannelInvitationTestFactory.h"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

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
    server::tests::UserTestFactory userTestFactory;
    server::tests::ChannelTestFactory channelTestFactory;
    server::tests::ChannelInvitationTestFactory channelInvitationTestFactory;

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
    const auto user = userTestFactory.createDomainUser();
    const auto recipient = userTestFactory.createDomainUser();
    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    const auto channel1 = channelTestFactory.createDomainChannel(user);
    const auto channel2 = channelTestFactory.createDomainChannel(user);

    const auto channelInvitation1 =
        channelInvitationTestFactory.createDomainChannelInvitation(user, recipient, channel1);
    const auto channelInvitation2 =
        channelInvitationTestFactory.createDomainChannelInvitation(user, recipient, channel2);

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedChannelInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedChannelInvitationsQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindReceivedChannelInvitationsQueryHandlerResult{
            {*channelInvitation1, *channelInvitation2}}));

    auto responseMessage = getUserChannelInvitationsMessageHandler.handleMessage(message);

    auto fewChannelResponsePayloadJson = nlohmann::json{
        {"data", nlohmann::json::array({{{"id", channelInvitation1->getId()}, {"name", channel1->getName()}},
                                        {{"id", channelInvitation2->getId()}, {"name", channel2->getName()}}})}};

    auto fewChannelMessageResponse =
        common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                                  common::bytes::Bytes{fewChannelResponsePayloadJson.dump()}};
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
