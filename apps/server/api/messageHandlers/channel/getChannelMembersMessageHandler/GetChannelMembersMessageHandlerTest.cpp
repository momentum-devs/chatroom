#include "GetChannelMembersMessageHandler.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "server/application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

using namespace ::testing;
using namespace server::api;
using namespace server::application;

namespace
{
auto token = "token";
auto channelId = "channelId";
auto validPayloadJson = nlohmann::json{{"token", token}, {"data", {{"channelId", channelId}}}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetChannelMembers, validPayload};

auto userId = "userId";
auto channelMemberId1 = "channelMemberId1";
auto channelMemberName1 = "channelMemberName1";
auto channelMemberActive1 = true;
auto channelMemberId2 = "channelMemberId2";
auto channelMemberName2 = "channelMemberName2";
auto channelMemberActive2 = false;
auto channelMembers = std::vector<server::domain::User>{
    {channelMemberId1, channelMemberName1, channelMemberActive1},
    {channelMemberId2, channelMemberName2, channelMemberActive2}
};

class GetChannelMembersMessageHandlerTest: public Test{
public:
    std::unique_ptr<FindUsersBelongingToChannelQueryHandlerMock> findUsersBelongingToChannelQueryHandlerMock = 
        std::make_unique<StrictMock<FindUsersBelongingToChannelQueryHandlerMock>>();
    FindUsersBelongingToChannelQueryHandlerMock* findUsersBelongingToChannelQueryHandlerMockPtr = 
        findUsersBelongingToChannelQueryHandlerMock.get();

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    GetChannelMembersMessageHandler getChannelMembersMessageHandler{
        tokenServiceMock, std::move(findUsersBelongingToChannelQueryHandlerMock)};
}

TEST_F(GetChannelMembersMessageHandlerTest, handleValidGetChannelMembersMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(std::make_tuple(userId)));

    EXPECT_CALL(*findUsersBelongingToChannelQueryHandlerMockPtr, execute(channelId))
        .WillOnce(Return(std::make_tuple(channelMembers)));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    nlohmann::json expectedResponsePayload{{"data", {
        {{"id", channelMemberId1}, {"name", channelMemberName1}, {"isActive", channelMemberActive1}},
        {{"id", channelMemberId2}, {"name", channelMemberName2}, {"isActive", channelMemberActive2}}
    }}};

    EXPECT_EQ(responseMessage.id, common::messages::MessageId::GetChannelMembersResponse);
    EXPECT_EQ(responseMessage.payload, common::bytes::Bytes{expectedResponsePayload.dump()});
}

TEST_F(GetChannelMembersMessageHandlerTest, handleGetChannelMembersMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(std::runtime_error("Invalid token")));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    nlohmann::json expectedResponsePayload{{"error", "Invalid token"}};

    EXPECT_EQ(responseMessage.id, common::messages::MessageId::GetChannelMembersResponse);
    EXPECT_EQ(responseMessage.payload, common::bytes::Bytes{expectedResponsePayload.dump()});
}

TEST_F(GetChannelMembersMessageHandlerTest, handleGetChannelMembersMessageWithException)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(std::make_tuple(userId)));

    EXPECT_CALL(*findUsersBelongingToChannelQueryHandlerMockPtr, execute(channelId))
        .WillOnce(Throw(std::runtime_error("Error while executing query")));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    nlohmann::json expectedResponsePayload{{"error", "Error while executing query"}};

    EXPECT_EQ(responseMessage.id, common::messages::MessageId::GetChannelMembersResponse);
    EXPECT_EQ(responseMessage.payload, common::bytes::Bytes{expectedResponsePayload.dump()});
}