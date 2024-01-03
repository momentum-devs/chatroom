#include "DeleteUserMessageHandler.h"

#include "fmt/format.h"
#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::DeleteUser, validPayload};
auto validMessageResponse =
    common::messages::Message{common::messages::MessageId::DeleteUserResponse, common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::DeleteUserResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error deleteUserError("deleteUserError");
auto deleteUserErrorMessageResponse = common::messages::Message{common::messages::MessageId::DeleteUserResponse,
                                                                common::bytes::Bytes{R"({"error":"deleteUserError"})"}};
}

class DeleteUserMessageHandlerTest : public Test
{
public:
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::DeleteUserCommandHandlerMock> deleteUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::DeleteUserCommandHandlerMock>>();
    server::application::DeleteUserCommandHandlerMock* deleteUserCommandHandlerMock =
        deleteUserCommandHandlerMockInit.get();

    DeleteUserMessageHandler deleteUserMessageHandler{tokenServiceMock, std::move(deleteUserCommandHandlerMockInit)};
};

TEST_F(DeleteUserMessageHandlerTest, handleValidDeleteUserMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteUserCommandHandlerMock, execute(server::application::DeleteUserCommandHandlerPayload{userId}));

    auto responseMessage = deleteUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(DeleteUserMessageHandlerTest, handleDeleteUserMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = deleteUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(DeleteUserMessageHandlerTest, handleDeleteUserMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*deleteUserCommandHandlerMock, execute(server::application::DeleteUserCommandHandlerPayload{userId}))
        .WillOnce(Throw(deleteUserError));

    auto responseMessage = deleteUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, deleteUserErrorMessageResponse);
}
