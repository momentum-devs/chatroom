#include "SendMessageToChannelMessageHandler.h"

#include <gtest/gtest.h>

#include "server/application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Datatype.h"
#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"

using namespace server::api;
using namespace ::testing;

namespace
{

const auto messageContent = "text";
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
    static std::shared_ptr<server::domain::User> createUser()
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
        const auto avatarUrl = faker::Image::imageUrl();

        return std::make_shared<server::domain::User>(
            server::domain::User{userId, userEmail, userPassword, userNickname, userActive, userEmailVerified,
                                 verificationCode, userCreatedAt, userUpdatedAt, avatarUrl});
    }

    std::string token = faker::String::alphanumeric(40);
    std::string channelId = faker::String::uuid();
    std::string senderId = faker::String::uuid();
    server::application::VerifyTokenResult verifyTokenResult{senderId};
    std::string text = faker::Lorem::sentence();
    nlohmann::json validPayloadJson{{"data", nlohmann::json{{"channelId", channelId}, {"text", text}}},
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

    SendMessageToChannelMessageHandler sendMessageToChannelMessageHandler{tokenServiceMock,
                                                        std::move(createChannelMessageCommandHandlerMockInit)};
};

TEST_F(SendMessageToChannelMessageHandlerTest, handleValidSendChannelMessage)
{
    const auto messageSender = createUser();
    const std::shared_ptr<server::domain::Channel> messageChannel = nullptr;
    const std::shared_ptr<server::domain::Group> messageGroup = nullptr;
    const auto messageCreatedAt = faker::Date::recentDate();
    const auto messageUpdatedAt = faker::Date::recentDate();
    const auto messageId = faker::String::uuid();
    server::domain::Message message{messageId,    messageContent,   messageSender,   messageChannel,
                                    messageGroup, messageCreatedAt, messageUpdatedAt};
    const auto validCommandHandlerResponse = server::application::CreateChannelMessageCommandHandlerResult{message};
    const auto validCommandHandlerResponseJson = nlohmann::json{{"data",
                                                                 {{"message",
                                                                   {{"id", messageId},
                                                                    {"text", messageContent},
                                                                    {"senderName", messageSender->getNickname()},
                                                                    {"sentAt", messageCreatedAt}}}}}};
    const auto validMessageResponse =
        common::messages::Message{common::messages::MessageId::SendChannelMessageResponse,
                                  common::bytes::Bytes{validCommandHandlerResponseJson.dump()}};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*createChannelMessageCommandHandlerMock,
                execute(server::application::CreateChannelMessageCommandHandlerPayload{text, senderId, channelId}))
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
    EXPECT_CALL(*createChannelMessageCommandHandlerMock,
                execute(server::application::CreateChannelMessageCommandHandlerPayload{text, senderId, channelId}))
        .WillOnce(Throw(createChannelMessageCommandHandlerError));

    auto responseMessage = sendMessageToChannelMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, createChannelMessageCommandHandlerErrorMessageResponse);
}
