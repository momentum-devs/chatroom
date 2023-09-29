#include "CreateChannelMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto channelName = "channelName";
auto token = "token";
auto creatorId = "id";
auto validPayloadJson = nlohmann::json{{"data", nlohmann::json{{"channelName", channelName}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::CreateChannel, validPayload};
auto validMessageResponse =
    common::messages::Message{common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::CreateChannelResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error createChannelError("createChannelError");
auto createChannelErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{R"({"error":"createChannelError"})"}};
}

class CreateChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::CreateChannelCommandHandlerMock> createChannelCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::CreateChannelCommandHandlerMock>>();
    server::application::CreateChannelCommandHandlerMock* createChannelCommandHandlerMock =
        createChannelCommandHandlerMockInit.get();

    CreateChannelMessageHandler createChannelMessageHandler{tokenServiceMock,
                                                            std::move(createChannelCommandHandlerMockInit)};
};

TEST_F(CreateChannelMessageHandlerTest, handleValidCreateChannelMessage)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(creatorId));
    EXPECT_CALL(*createChannelCommandHandlerMock,
                execute(server::application::CreateChannelCommandHandlerPayload{channelName, creatorId}))
        .WillOnce(Return(server::application::CreateChannelCommandHandlerResult{{"", "", {}, "", ""}}));

    auto responseMessage = createChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(CreateChannelMessageHandlerTest, handleCreateChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = createChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(CreateChannelMessageHandlerTest, handleCreateChannelMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(creatorId));
    EXPECT_CALL(*createChannelCommandHandlerMock,
                execute(server::application::CreateChannelCommandHandlerPayload{channelName, creatorId}))
        .WillOnce(Throw(createChannelError));

    auto responseMessage = createChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, createChannelErrorMessageResponse);
}
