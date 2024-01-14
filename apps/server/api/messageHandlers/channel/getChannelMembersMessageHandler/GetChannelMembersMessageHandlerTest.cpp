#include "GetChannelMembersMessageHandler.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "server/application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "fmt/format.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;
using namespace server::application;

class GetChannelMembersMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    std::string token = "token";
    std::string channelId = "channelId";
    FindUsersBelongingToChannelQueryHandlerPayload queryHandlerPayload{channelId};
    nlohmann::json validPayloadJson{{"token", token}, {"data", {{"channelId", channelId}}}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message message{common::messages::MessageId::GetChannelMembers, validPayload};

    server::application::VerifyTokenResult verifyTokenResult{"userId"};

    std::unique_ptr<FindUsersBelongingToChannelQueryHandlerMock> findUsersBelongingToChannelQueryHandlerMock =
        std::make_unique<StrictMock<FindUsersBelongingToChannelQueryHandlerMock>>();
    FindUsersBelongingToChannelQueryHandlerMock* findUsersBelongingToChannelQueryHandlerMockPtr =
        findUsersBelongingToChannelQueryHandlerMock.get();

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    GetChannelMembersMessageHandler getChannelMembersMessageHandler{
        tokenServiceMock, std::move(findUsersBelongingToChannelQueryHandlerMock)};
};

TEST_F(GetChannelMembersMessageHandlerTest, handleValidGetChannelMembersMessage)
{
    auto channelMember1 = userTestFactory.createDomainUser();
    auto channelMember2 = userTestFactory.createDomainUser();

    std::vector<server::domain::User> channelMembers{
        *channelMember1,
        *channelMember2,
    };

    FindUsersBelongingToChannelQueryHandlerResult queryHandlerResult{channelMembers};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));

    EXPECT_CALL(*findUsersBelongingToChannelQueryHandlerMockPtr, execute(queryHandlerPayload))
        .WillOnce(Return(queryHandlerResult));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    const auto validMessageResponsePayload =
        R"({"data":[)" +
        fmt::format(R"({{"id":"{}","name":"{}","isActive":{}}},)", channelMember1->getId(),
                    channelMember1->getNickname(), channelMember1->isActive()) +
        fmt::format(R"({{"id":"{}","name":"{}","isActive":{}}})", channelMember2->getId(),
                    channelMember2->getNickname(), channelMember2->isActive()) +
        R"(]})";

    const auto expectedResponsePayload = nlohmann::json::parse(validMessageResponsePayload);
    const auto expectedResponseMessage = common::messages::Message{
        common::messages::MessageId::GetChannelMembersResponse, common::bytes::Bytes{expectedResponsePayload.dump()}};

    EXPECT_EQ(responseMessage, expectedResponseMessage);
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
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));

    EXPECT_CALL(*findUsersBelongingToChannelQueryHandlerMockPtr, execute(queryHandlerPayload))
        .WillOnce(Throw(std::runtime_error("Error while executing query")));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    nlohmann::json expectedResponsePayload{{"error", "Error while executing query"}};

    EXPECT_EQ(responseMessage.id, common::messages::MessageId::GetChannelMembersResponse);
    EXPECT_EQ(responseMessage.payload, common::bytes::Bytes{expectedResponsePayload.dump()});
}
