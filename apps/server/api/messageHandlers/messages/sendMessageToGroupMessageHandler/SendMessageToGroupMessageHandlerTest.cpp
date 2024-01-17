#include "SendMessageToGroupMessageHandler.h"

#include <gtest/gtest.h>

#include "server/application/commandHandlers/message/createGroupMessageCommandHandler/CreateGroupMessageCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace server::api;
using namespace ::testing;

namespace
{
const auto invalidTokenError = std::runtime_error{"invalidToken"};
const auto invalidTokenMessageResponse = common::messages::Message{
    common::messages::MessageId::SendPrivateMessageResponse, common::bytes::Bytes{R"({"error":"invalidToken"})"}};

const auto createChannelMessageCommandHandlerError = std::runtime_error{"createGroupMessageCommandHandlerError"};
const auto createChannelMessageCommandHandlerErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendPrivateMessageResponse,
                              common::bytes::Bytes{R"({"error":"createGroupMessageCommandHandlerError"})"}};
}

class SendMessageToGroupMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    server::tests::GroupTestFactory groupTestFactory;
    server::tests::MessageTestFactory messageTestFactory;

    std::string token = faker::String::alphanumeric(40);
    const std::shared_ptr<server::domain::User> messageSender = userTestFactory.createDomainUser();
    server::application::VerifyTokenResult verifyTokenResult{messageSender->getId()};

    const std::shared_ptr<server::domain::Group> messageGroup = groupTestFactory.createDomainGroup();
    const std::shared_ptr<server::domain::Message> message =
        messageTestFactory.createDomainMessage(messageSender, nullptr, messageGroup);
    nlohmann::json validPayloadJson{
        {"data", nlohmann::json{{"groupId", messageGroup->getId()}, {"text", message->getContent()}}},
        {"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message validMessage{common::messages::MessageId::SendPrivateMessage, validPayload};

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::CreateGroupMessageCommandHandlerMock>
        createGroupMessageCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateGroupMessageCommandHandlerMock>>();

    server::application::CreateGroupMessageCommandHandlerMock* createGroupMessageCommandHandlerMock =
        createGroupMessageCommandHandlerMockInit.get();

    SendMessageToGroupMessageHandler sendMessageToGroupMessageHandler{
        tokenServiceMock, std::move(createGroupMessageCommandHandlerMockInit)};
};

TEST_F(SendMessageToGroupMessageHandlerTest, handleValidSendChannelMessage)
{
    const auto validCommandHandlerResponse = server::application::CreateGroupMessageCommandHandlerResult{*message};
    const auto validCommandHandlerResponseJson = nlohmann::json{{"data",
                                                                 {{"message",
                                                                   {{"id", message->getId()},
                                                                    {"text", message->getContent()},
                                                                    {"senderName", messageSender->getNickname()},
                                                                    {"sentAt", message->getCreatedAt()}}}}}};
    auto commandPayload = server::application::CreateGroupMessageCommandHandlerPayload{
        message->getContent(), messageSender->getId(), messageGroup->getId()};
    nlohmann::json validPayloadJson{
        {"data", nlohmann::json{{"groupId", messageGroup->getId()}, {"text", message->getContent()}}},
        {"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message validMessage{common::messages::MessageId::SendPrivateMessage, validPayload};

    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::SendPrivateMessageResponse,
                                  common::bytes::Bytes{validCommandHandlerResponseJson.dump()}};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*createGroupMessageCommandHandlerMock, execute(commandPayload))
        .WillOnce(Return(validCommandHandlerResponse));

    auto responseMessage = sendMessageToGroupMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(SendMessageToGroupMessageHandlerTest, handleSendChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidTokenError));

    auto responseMessage = sendMessageToGroupMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(SendMessageToGroupMessageHandlerTest, handleSendChannelMessageWithCreateChannelMessageCommandHandlerError)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    auto commandPayload = server::application::CreateGroupMessageCommandHandlerPayload{
        message->getContent(), messageSender->getId(), messageGroup->getId()};
    EXPECT_CALL(*createGroupMessageCommandHandlerMock, execute(commandPayload))
        .WillOnce(Throw(createChannelMessageCommandHandlerError));

    auto responseMessage = sendMessageToGroupMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, createChannelMessageCommandHandlerErrorMessageResponse);
}
