#include "GetMessagesFromGroupMessageHandler.h"

#include <format>
#include <gtest/gtest.h>

#include "server/application/queryHandlers/message/findGroupMessagesQueryHandler/FindGroupMessagesQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace server::api;
using namespace ::testing;
using namespace server::tests;

namespace
{
const auto invalidTokenError = std::runtime_error{"invalidToken"};
const auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::GetPrivateMessagesResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};
}

class GetMessagesFromGroupMessageHandlerTest : public Test
{
public:
    UserTestFactory userFactory;
    ChannelTestFactory channelFactory;
    MessageTestFactory messageFactory;
    GroupTestFactory groupFactory;
    const std::string token = faker::String::alphanumeric(40);
    const std::string groupId = faker::String::uuid();
    const std::string senderId = faker::String::uuid();
    const server::application::VerifyTokenResult verifyTokenResult{senderId};
    const std::shared_ptr<server::domain::Message> message1 = messageFactory.createDomainMessage(
        userFactory.createDomainUser(), channelFactory.createDomainChannel(userFactory.createDomainUser()),
        groupFactory.createDomainGroup());
    const std::shared_ptr<server::domain::Message> message2 = messageFactory.createDomainMessage(
        userFactory.createDomainUser(), channelFactory.createDomainChannel(userFactory.createDomainUser()),
        groupFactory.createDomainGroup());
    const std::vector<server::domain::Message> messages{*message1, *message2};
    const unsigned totalCount = 2;

    const nlohmann::json validPayloadJson{{"data", nlohmann::json{{"groupId", groupId}, {"limit", 10}, {"offset", 0}}},
                                          {"token", token}};
    const common::bytes::Bytes validPayload{validPayloadJson.dump()};
    const common::messages::Message validMessage{common::messages::MessageId::GetPrivateMessagesResponse, validPayload};

    const std::string validMessageResponsePayload =
        R"({"data":{"messages":[)" +
        std::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}},)", message1->getId(),
                    message1->getSender()->getNickname(), message1->getCreatedAt(), message1->getContent()) +
        std::format(R"({{"id":"{}","senderName":"{}","sentAt":"{}","text":"{}"}})", message2->getId(),
                    message2->getSender()->getNickname(), message2->getCreatedAt(), message2->getContent()) +
        R"(],"totalCount":2}})";
    const common::messages::Message validMessageResponse{common::messages::MessageId::GetPrivateMessagesResponse,
                                                         common::bytes::Bytes{validMessageResponsePayload}};
    const server::application::FindGroupMessagesQueryHandlerResult validCommandHandlerResponse{messages, totalCount};

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindGroupMessagesQueryHandlerMock> findGroupMessagesQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindGroupMessagesQueryHandlerMock>>();

    server::application::FindGroupMessagesQueryHandlerMock* findGroupMessagesQueryHandlerMock =
        findGroupMessagesQueryHandlerMockInit.get();

    GetMessagesFromGroupMessageHandler getMessagesFromGroupMessageHandler{
        tokenServiceMock, std::move(findGroupMessagesQueryHandlerMockInit)};
};

TEST_F(GetMessagesFromGroupMessageHandlerTest, handleMessage_shouldReturnValidMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findGroupMessagesQueryHandlerMock, execute(_)).WillOnce(Return(validCommandHandlerResponse));

    const auto messageResponse = getMessagesFromGroupMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetMessagesFromGroupMessageHandlerTest, handleMessage_shouldReturnInvalidTokenMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidTokenError));

    const auto messageResponse = getMessagesFromGroupMessageHandler.handleMessage(validMessage);

    ASSERT_EQ(messageResponse, invalidTokenMessageResponse);
}

TEST_F(GetMessagesFromGroupMessageHandlerTest, handleMessage_shouldReturnValidMessageResponseWithEmptyMessages)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findGroupMessagesQueryHandlerMock, execute(_))
        .WillOnce(Return(server::application::FindGroupMessagesQueryHandlerResult{{}, 0}));

    const auto messageResponse = getMessagesFromGroupMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetPrivateMessagesResponse,
                                  common::bytes::Bytes{R"({"data":{"messages":[],"totalCount":0}})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}

TEST_F(GetMessagesFromGroupMessageHandlerTest, handleMessage_shouldReturnErrorMessageResponse)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findGroupMessagesQueryHandlerMock, execute(_))
        .WillOnce(Throw(std::runtime_error{"findChannelMessagesQueryHandlerError"}));

    const auto messageResponse = getMessagesFromGroupMessageHandler.handleMessage(validMessage);

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::GetPrivateMessagesResponse,
                                  common::bytes::Bytes{R"({"error":"findChannelMessagesQueryHandlerError"})"}};
    ASSERT_EQ(messageResponse, validMessageResponse);
}
