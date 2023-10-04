#include "DeleteTheChannelMessageHandler.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
auto channelId = "channel_id";
auto validPayloadJson = nlohmann::json{{"data", {{"channelId", channelId}}}, {"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::DeleteTheChannel, validPayload};
auto validMessageResponse =
    common::messages::Message{common::messages::MessageId::DeleteTheChannelResponse, common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::DeleteTheChannelResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error deleteChannelError("deleteChannelError");
auto deleteUserErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::DeleteTheChannelResponse, common::bytes::Bytes{R"({"error":"deleteChannelError"})"}};
}

class DeleteTheChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::DeleteChannelCommandHandlerMock> deleteChannelCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::DeleteChannelCommandHandlerMock>>();
    server::application::DeleteChannelCommandHandlerMock* deleteChannelCommandHandlerMock =
        deleteChannelCommandHandlerMockInit.get();

    DeleteTheChannelMessageHandler deleteTheChannelMessageHandler{tokenServiceMock,
                                                                  std::move(deleteChannelCommandHandlerMockInit)};
};

TEST_F(DeleteTheChannelMessageHandlerTest, handleValidDeleteTheChannelMessage)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*deleteChannelCommandHandlerMock,
                execute(server::application::DeleteChannelCommandHandlerPayload{channelId, userId}));

    auto responseMessage = deleteTheChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(DeleteTheChannelMessageHandlerTest, handleDeleteTheChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = deleteTheChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(DeleteTheChannelMessageHandlerTest, handleDeleteTheChannelMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(userId));
    EXPECT_CALL(*deleteChannelCommandHandlerMock,
                execute(server::application::DeleteChannelCommandHandlerPayload{channelId, userId}))
        .WillOnce(Throw(deleteChannelError));

    auto responseMessage = deleteTheChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, deleteUserErrorMessageResponse);
}
