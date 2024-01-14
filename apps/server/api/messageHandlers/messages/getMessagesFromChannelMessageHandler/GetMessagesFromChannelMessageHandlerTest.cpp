#include "GetMessagesFromChannelMessageHandler.h"

#include <gtest/gtest.h>

#include "server/application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Number.h"
#include "faker-cxx/String.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace server::api;
using namespace ::testing;
using namespace server::tests;

namespace
{
const auto invalidTokenError = std::runtime_error{"invalidToken"};
const auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::GetChannelMessagesResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};
}

class GetMessagesFromChannelMessageHandlerTest : public Test
{
public:
    UserTestFactory userFactory;
    ChannelTestFactory channelFactory;
    MessageTestFactory messageFactory;

    const std::string token = faker::String::alphanumeric(40);
    const std::string senderId = faker::String::uuid();
    const unsigned limit = faker::Number::integer(1, 100);
    const unsigned offset = faker::Number::integer(1, 100);
    const server::application::VerifyTokenResult verifyTokenResult{senderId};
    const std::shared_ptr<server::domain::Channel> channel =
        channelFactory.createDomainChannel(userFactory.createDomainUser());
    const std::shared_ptr<server::domain::Message> message1 =
        messageFactory.createDomainMessage(userFactory.createDomainUser(), channel, nullptr);
    const std::shared_ptr<server::domain::Message> message2 =
        messageFactory.createDomainMessage(userFactory.createDomainUser(), channel, nullptr);
    const std::vector<server::domain::Message> messages{*message1, *message2};
    const unsigned totalCount = 2;

    const nlohmann::json validPayloadJson{
        {"data", nlohmann::json{{"channelId", channel->getId()}, {"limit", limit}, {"offset", offset}}},
        {"token", token}};
    const common::bytes::Bytes validPayload{validPayloadJson.dump()};
    const common::messages::Message validMessage{common::messages::MessageId::GetChannelMessages, validPayload};

    server::application::FindChannelMessagesQueryHandlerPayload queryPayload{channel->getId(), offset, limit};
    const std::string validMessageResponsePayload =
        R"({"data":{"messages":[)" +
        fmt::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}},)", message1->getId(),
                    message1->getSender()->getNickname(), message1->getCreatedAt(), message1->getContent()) +
        fmt::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}})", message2->getId(),
                    message2->getSender()->getNickname(), message2->getCreatedAt(), message2->getContent()) +
        R"(],"totalCount":2}})";
    const common::messages::Message validMessageResponse{common::messages::MessageId::GetChannelMessagesResponse,
                                                         common::bytes::Bytes{validMessageResponsePayload}};
    const server::application::FindChannelMessagesQueryHandlerResult validCommandHandlerResponse{messages, totalCount};

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindChannelMessagesQueryHandlerMock> findChannelMessagesQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindChannelMessagesQueryHandlerMock>>();

    server::application::FindChannelMessagesQueryHandlerMock* findChannelMessagesQueryHandlerMock =
        findChannelMessagesQueryHandlerMockInit.get();

    GetMessagesFromChannelMessageHandler getMessagesFromChannelMessageHandler{
        tokenServiceMock, std::move(findChannelMessagesQueryHandlerMockInit)};
};

TEST_F(GetMessagesFromChannelMessageHandlerTest, handleMessage_shouldReturnValidMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(queryPayload))
        .WillOnce(Return(validCommandHandlerResponse));

    const auto messageResponse = getMessagesFromChannelMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetMessagesFromChannelMessageHandlerTest, handleMessage_shouldReturnInvalidTokenMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidTokenError));

    const auto messageResponse = getMessagesFromChannelMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, invalidTokenMessageResponse);
}

TEST_F(GetMessagesFromChannelMessageHandlerTest, handleMessage_shouldReturnValidMessageResponseWithEmptyMessages)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(queryPayload))
        .WillOnce(Return(server::application::FindChannelMessagesQueryHandlerResult{{}, 0}));

    const auto messageResponse = getMessagesFromChannelMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetChannelMessagesResponse,
                                  common::bytes::Bytes{R"({"data":{"messages":[],"totalCount":0}})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetMessagesFromChannelMessageHandlerTest, handleMessage_shouldReturnErrorMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findChannelMessagesQueryHandlerMock, execute(queryPayload))
        .WillOnce(Throw(std::runtime_error{"findChannelMessagesQueryHandlerError"}));

    const auto messageResponse = getMessagesFromChannelMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetChannelMessagesResponse,
                                  common::bytes::Bytes{R"({"error":"findChannelMessagesQueryHandlerError"})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}
