#include "SendChannelMessageHandler.h"

#include <gtest/gtest.h>

#include "server/application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"

using namespace server::api;
using namespace ::testing;

namespace
{
const auto token = faker::String::alphanumeric(40);
const auto channelId = faker::String::uuid();
const auto senderId = faker::String::uuid();
const auto verifyTokenResult = server::application::VerifyTokenResult{senderId};
const auto text = faker::Lorem::sentence();
const auto validPayloadJson =
    nlohmann::json{{"data", nlohmann::json{{"channelId", channelId}, {"text", text}}}, {"token", token}};
const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
const auto validMessage = common::messages::Message{common::messages::MessageId::SendChannelMessage, validPayload};
const auto messageId = faker::String::uuid();
const auto messageContent = text;
const std::shared_ptr<server::domain::User> messageSender = nullptr;
const std::shared_ptr<server::domain::Channel> messageChannel = nullptr;
const std::shared_ptr<server::domain::Group> messageGroup = nullptr;
const auto messageCreatedAt = faker::Date::recentDate();
const auto messageUpdatedAt = faker::Date::recentDate();
server::domain::Message message{messageId,    messageContent,   messageSender,   messageChannel,
                                messageGroup, messageCreatedAt, messageUpdatedAt};
const auto validCommandHandlerResponse = server::application::CreateChannelMessageCommandHandlerResult{message};
const auto validMessageResponse = common::messages::Message{common::messages::MessageId::SendChannelMessageResponse,
                                                            common::bytes::Bytes{R"(["ok"])"}};
}

class SendChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::CreateChannelMessageCommandHandlerMock>
        createChannelMessageCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateChannelMessageCommandHandlerMock>>();

    server::application::CreateChannelMessageCommandHandlerMock* createChannelMessageCommandHandlerMock =
        createChannelMessageCommandHandlerMockInit.get();

    SendChannelMessageHandler sendChannelMessageHandler{tokenServiceMock,
                                                        std::move(createChannelMessageCommandHandlerMockInit)};
};

TEST_F(SendChannelMessageHandlerTest, handleValidSendChannelMessage)
{

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*createChannelMessageCommandHandlerMock,
                execute(server::application::CreateChannelMessageCommandHandlerPayload{text, senderId, channelId}))
        .WillOnce(Return(validCommandHandlerResponse));

    auto responseMessage = sendChannelMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, validMessageResponse);
}