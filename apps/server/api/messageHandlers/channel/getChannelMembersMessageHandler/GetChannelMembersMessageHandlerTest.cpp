#include "GetChannelMembersMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "server/application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

using namespace ::testing;
using namespace server::api;
using namespace server::application;

namespace
{
server::domain::User createUser()
{
    const auto userId = faker::String::uuid();
    const auto userNickname = faker::String::alphanumeric(10);
    const auto userEmail = faker::Internet::email();
    const auto userPassword = faker::String::alphanumeric(10);
    const auto userCreatedAt = faker::Date::recentDate();
    const auto userUpdatedAt = faker::Date::recentDate();
    const bool userActive = faker::Datatype::boolean();
    const bool userEmailVerified = faker::Datatype::boolean();
    const auto verificationCode = faker::String::numeric(6);
    return {userId,           userEmail,     userPassword, userNickname, userActive, userEmailVerified,
            verificationCode, userCreatedAt, userUpdatedAt};
}

const auto token = "token";
const auto channelId = "channelId";
const auto queryHandlerPayload = FindUsersBelongingToChannelQueryHandlerPayload{channelId};
const auto validPayloadJson = nlohmann::json{{"token", token}, {"data", {{"channelId", channelId}}}};
const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
const auto message = common::messages::Message{common::messages::MessageId::GetChannelMembers, validPayload};

const auto userId = "userId";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
const auto channelMember1 = createUser();
const auto channelMember2 = createUser();
const auto channelMembers = std::vector<server::domain::User>{
    channelMember1,
    channelMember2,
};
const auto queryHandlerResult = FindUsersBelongingToChannelQueryHandlerResult{channelMembers};
}

class GetChannelMembersMessageHandlerTest : public Test
{
public:
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
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));

    EXPECT_CALL(*findUsersBelongingToChannelQueryHandlerMockPtr, execute(queryHandlerPayload))
        .WillOnce(Return(queryHandlerResult));

    auto responseMessage = getChannelMembersMessageHandler.handleMessage(message);

    const auto validMessageResponsePayload =
        R"({"data":[)" +
        std::format(R"({{"id":"{}","name":"{}","isActive":{}}},)", channelMember1.getId(), channelMember1.getNickname(),
                    channelMember1.isActive()) +
        std::format(R"({{"id":"{}","name":"{}","isActive":{}}})", channelMember2.getId(), channelMember2.getNickname(),
                    channelMember2.isActive()) +
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
