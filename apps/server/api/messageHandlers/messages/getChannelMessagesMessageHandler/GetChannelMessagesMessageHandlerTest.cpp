#include "GetChannelMessagesMessageHandler.h"

#include <format>
#include <gtest/gtest.h>

#include "server/application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"

using namespace server::api;
using namespace ::testing;

namespace
{

// TODO: move it to common test utils
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

server::domain::Message createMessage()
{
    const auto messageId = faker::String::uuid();
    const auto messageContent = faker::Lorem::sentence();
    const auto messageSender = std::make_shared<server::domain::User>(createUser());
    const std::shared_ptr<server::domain::Channel> messageChannel = nullptr;
    const std::shared_ptr<server::domain::Group> messageGroup = nullptr;
    const auto messageCreatedAt = faker::Date::recentDate();
    const auto messageUpdatedAt = faker::Date::recentDate();
    return server::domain::Message{messageId,    messageContent,   messageSender,   messageChannel,
                                   messageGroup, messageCreatedAt, messageUpdatedAt};
}

const auto token = faker::String::alphanumeric(40);
const auto channelId = faker::String::uuid();
const auto senderId = faker::String::uuid();
const auto verifyTokenResult = server::application::VerifyTokenResult{senderId};
const auto message1 = createMessage();
const auto message2 = createMessage();
const auto messages = std::vector<server::domain::Message>{message1, message2};
const auto totalCount = 2;

const auto validPayloadJson =
    nlohmann::json{{"data", nlohmann::json{{"channelId", channelId}, {"limit", 10}, {"offset", 0}}}, {"token", token}};
const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
const auto validMessage = common::messages::Message{common::messages::MessageId::GetChannelMessages, validPayload};

const auto validCommandHandlerResponse =
    server::application::FindChannelMessagesQueryHandlerResult{messages, totalCount};
const auto validMessageResponsePayload =
    R"({"data":{"messages":[)" +
    std::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}},)", message1.getId(),
                message1.getSender()->getNickname(), message1.getCreatedAt(), message1.getContent()) +
    std::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}})", message2.getId(),
                message2.getSender()->getNickname(), message2.getCreatedAt(), message2.getContent()) +
    R"(],"totalCount":2}})";
const auto validMessageResponse = common::messages::Message{common::messages::MessageId::GetChannelMessagesResponse,
                                                            common::bytes::Bytes{validMessageResponsePayload}};

const auto invalidTokenError = std::runtime_error{"invalidToken"};
const auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::GetChannelMessagesResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};
}

class GetChannelMessagesMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindChannelMessagesQueryHandlerMock> findChannelMessagesQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindChannelMessagesQueryHandlerMock>>();

    server::application::FindChannelMessagesQueryHandlerMock* findChannelMessagesQueryHandlerMock =
        findChannelMessagesQueryHandlerMockInit.get();

    GetChannelMessagesMessageHandler getChannelMessagesMessageHandler{
        tokenServiceMock, std::move(findChannelMessagesQueryHandlerMockInit)};
};

TEST_F(GetChannelMessagesMessageHandlerTest, handleMessage_shouldReturnValidMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(_)).WillOnce(Return(validCommandHandlerResponse));

    const auto messageResponse = getChannelMessagesMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetChannelMessagesMessageHandlerTest, handleMessage_shouldReturnInvalidTokenMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidTokenError));

    const auto messageResponse = getChannelMessagesMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, invalidTokenMessageResponse);
}

TEST_F(GetChannelMessagesMessageHandlerTest, handleMessage_shouldReturnValidMessageResponseWithEmptyMessages)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(_))
        .WillOnce(Return(server::application::FindChannelMessagesQueryHandlerResult{{}, 0}));

    const auto messageResponse = getChannelMessagesMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetChannelMessagesResponse,
                                  common::bytes::Bytes{R"({"data":{"messages":[],"totalCount":0}})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetChannelMessagesMessageHandlerTest, handleMessage_shouldReturnValidMessageResponseWithOneMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(_))
        .WillOnce(Throw(std::runtime_error{"findChannelMessagesQueryHandlerError"}));

    const auto messageResponse = getChannelMessagesMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetChannelMessagesResponse,
                                  common::bytes::Bytes{R"({"error":"findChannelMessagesQueryHandlerError"})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}