#include "DeleteChannelMessageHandler.h"

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
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
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

class DeleteChannelMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::DeleteChannelCommandHandlerMock> deleteChannelCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::DeleteChannelCommandHandlerMock>>();
    server::application::DeleteChannelCommandHandlerMock* deleteChannelCommandHandlerMock =
        deleteChannelCommandHandlerMockInit.get();

    DeleteChannelMessageHandler deleteChannelMessageHandler{tokenServiceMock,
                                                            std::move(deleteChannelCommandHandlerMockInit)};
};

TEST_F(DeleteChannelMessageHandlerTest, handleValidDeleteTheChannelMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteChannelCommandHandlerMock,
                execute(server::application::DeleteChannelCommandHandlerPayload{channelId, userId}));

    auto responseMessage = deleteChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(DeleteChannelMessageHandlerTest, handleDeleteTheChannelMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = deleteChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(DeleteChannelMessageHandlerTest, handleDeleteTheChannelMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteChannelCommandHandlerMock,
                execute(server::application::DeleteChannelCommandHandlerPayload{channelId, userId}))
        .WillOnce(Throw(deleteChannelError));

    auto responseMessage = deleteChannelMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, deleteUserErrorMessageResponse);
}
