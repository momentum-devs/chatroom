#include "UpdateUserMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "nlohmann/json.hpp"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
nlohmann::json validMessageResponsePayloadJson{"ok"};
common::messages::Message validMessageResponse{common::messages::MessageId::UpdateUserResponse,
                                               common::bytes::Bytes{validMessageResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
common::messages::Message invalidTokenMessageResponse{common::messages::MessageId::UpdateUserResponse,
                                                      common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error updateUserDataError("updateUserDataError");
common::messages::Message updateUserDataErrorMessageResponse{
    common::messages::MessageId::UpdateUserResponse, common::bytes::Bytes{R"({"error":"updateUserDataError"})"}};
}

class UpdateUserMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    std::shared_ptr<server::domain::User> user = userTestFactory.createDomainUser();
    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();
    const server::application::VerifyTokenResult verifyTokenResult{user->getId()};
    nlohmann::json validPayloadJson{{"data", {{"password", user->getPassword()}, {"nickname", user->getNickname()}}},
                                    {"token", token}};
    common::bytes::Bytes validPayload{validPayloadJson.dump()};
    common::messages::Message message{common::messages::MessageId::UpdateUser, validPayload};
    server::application::UpdateUserCommandHandlerPayload commandPayload{user->getId(), user->getNickname(),
                                                                        user->getPassword()};

    std::unique_ptr<server::application::UpdateUserCommandHandlerMock> updateUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::UpdateUserCommandHandlerMock>>();
    server::application::UpdateUserCommandHandlerMock* updateUserCommandHandlerMock =
        updateUserCommandHandlerMockInit.get();

    UpdateUserMessageHandler updateUserMessageHandler{tokenServiceMock, std::move(updateUserCommandHandlerMockInit)};
};

TEST_F(UpdateUserMessageHandlerTest, handleValidUpdateUserMessage)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*updateUserCommandHandlerMock, execute(commandPayload))
        .WillOnce(Return(server::application::UpdateUserCommandHandlerResult{*user}));
    
    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(UpdateUserMessageHandlerTest, handleUpdateUserMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(UpdateUserMessageHandlerTest, handleUpdateUserMessageWithErrorWhileHandling)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*updateUserCommandHandlerMock, execute(commandPayload)).WillOnce(Throw(updateUserDataError));

    auto responseMessage = updateUserMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, updateUserDataErrorMessageResponse);
}
