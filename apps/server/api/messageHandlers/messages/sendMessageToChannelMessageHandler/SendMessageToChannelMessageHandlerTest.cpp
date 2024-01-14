#include "SendMessageToChannelMessageHandler.h"

#include <gtest/gtest.h>

#include "server/application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace server::api;
using namespace ::testing;

namespace
{
const auto invalidTokenError = std::runtime_error{"invalidToken"};
const auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::SendChannelMessageResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

const auto createChannelMessageCommandHandlerError = std::runtime_error{"createChannelMessageCommandHandlerError"};
const auto createChannelMessageCommandHandlerErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendChannelMessageResponse,
                              common::bytes::Bytes{R"({"error":"createChannelMessageCommandHandlerError"})"}};
}

class SendMessageToChannelMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    server::tests::ChannelTestFactory channelTestFactory;
    server::tests::MessageTestFactory messageTestFactory;

    std::string token = faker::String::alphanumeric(40);
    const std::shared_ptr<server::domain::User> messageSender = userTestFactory.createDomainUser();
    server::application::VerifyTokenResult verifyTokenResult{messageSender->getId()};

    const std::shared_ptr<server::domain::Channel> messageChannel =
        channelTestFactory.createDomainChannel(messageSender);
    const std::shared_ptr<server::domain::Message> message =
        messageTestFactory.createDomainMessage(messageSender, messageChannel, nullptr);
    nlohmann::json validPayloadJson{
        {"data", nlohmann::json{{"channelId", messageChannel->getId()}, {"text", message->getContent()}}},
        {"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message validMessage{common::messages::MessageId::SendChannelMessage, validPayload};

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::CreateChannelMessageCommandHandlerMock>
        createChannelMessageCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateChannelMessageCommandHandlerMock>>();

    server::application::CreateChannelMessageCommandHandlerMock* createChannelMessageCommandHandlerMock =
        createChannelMessageCommandHandlerMockInit.get();

    SendMessageToChannelMessageHandler sendMessageToChannelMessageHandler{
        tokenServiceMock, std::move(createChannelMessageCommandHandlerMockInit)};
};

TEST_F(SendMessageToChannelMessageHandlerTest, handleValidSendChannelMessage)
{
    const auto validCommandHandlerResponse = server::application::CreateChannelMessageCommandHandlerResult{*message};
    const auto validCommandHandlerResponseJson = nlohmann::json{{"data",
                                                                 {{"message",
                                                                   {{"id", message->getId()},
                                                                    {"text", message->getContent()},
                                                                    {"senderName", messageSender->getNickname()},
                                                                    {"sentAt", message->getCreatedAt()}}}}}};
    auto commandPayload = server::application::CreateChannelMessageCommandHandlerPayload{
        message->getContent(), messageSender->getId(), messageChannel->getId()};
    nlohmann::json validPayloadJson{
        {"data", nlohmann::json{{"channelId", messageChannel->getId()}, {"text", message->getContent()}}},
        {"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message validMessage{common::messages::MessageId::SendChannelMessage, validPayload};

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::SendChannelMessageResponse,
                                  common::bytes::Bytes{validCommandHandlerResponseJson.dump()}};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*createChannelMessageCommandHandlerMock, execute(commandPayload))
        .WillOnce(Return(validCommandHandlerResponse));

    auto responseMessage = sendMessageToChannelMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(SendMessageToChannelMessageHandlerTest, handleSendChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidTokenError));

    auto responseMessage = sendMessageToChannelMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(SendMessageToChannelMessageHandlerTest, handleSendChannelMessageWithCreateChannelMessageCommandHandlerError)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    auto commandPayload = server::application::CreateChannelMessageCommandHandlerPayload{
        message->getContent(), messageSender->getId(), messageChannel->getId()};
    EXPECT_CALL(*createChannelMessageCommandHandlerMock, execute(commandPayload))
        .WillOnce(Throw(createChannelMessageCommandHandlerError));

    auto responseMessage = sendMessageToChannelMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, createChannelMessageCommandHandlerErrorMessageResponse);
}
