#include "FriendInvitationMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerMock.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Internet.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto userId = "id";
const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

auto validMessageResponse = common::messages::Message{common::messages::MessageId::SendFriendInvitationResponse,
                                                      common::bytes::Bytes{R"(["ok"])"}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::SendFriendInvitationResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error findFriendEmailError("findFriendEmailError");
auto findFriendEmailErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendFriendInvitationResponse,
                              common::bytes::Bytes{R"({"error":"findFriendEmailError"})"}};

std::runtime_error sendFriendInvitationError("sendFriendInvitationError");
auto sendFriendInvitationErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::SendFriendInvitationResponse,
                              common::bytes::Bytes{R"({"error":"sendFriendInvitationError"})"}};
}

class FriendInvitationMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::shared_ptr<server::application::FindUserByEmailQueryHandlerMock> findUserByEmailQueryHandler =
        std::make_shared<StrictMock<server::application::FindUserByEmailQueryHandlerMock>>();

    std::unique_ptr<server::application::CreateFriendInvitationCommandHandlerMock>
        createFriendInvitationCommandHandlerMockInit =
            std::make_unique<StrictMock<server::application::CreateFriendInvitationCommandHandlerMock>>();
    server::application::CreateFriendInvitationCommandHandlerMock* createFriendInvitationCommandHandlerMock =
        createFriendInvitationCommandHandlerMockInit.get();

    FriendInvitationMessageHandler friendInvitationMessageHandler{
        tokenServiceMock, findUserByEmailQueryHandler, std::move(createFriendInvitationCommandHandlerMockInit)};
};

TEST_F(FriendInvitationMessageHandlerTest, handleValidFriendInvitationMessage)
{
    const auto friendUser = userTestFactory.createDomainUser();
    const auto validPayloadJson =
        nlohmann::json{{"data", {{"friend_email", friendUser->getEmail()}}}, {"token", token}};
    const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
    const auto validMessage =
        common::messages::Message{common::messages::MessageId::SendFriendInvitation, validPayload};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendUser->getEmail()}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{*friendUser}));
    EXPECT_CALL(*createFriendInvitationCommandHandlerMock,
                execute(server::application::CreateFriendInvitationCommandHandlerPayload{userId, friendUser->getId()}));

    auto responseMessage = friendInvitationMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, validMessageResponse);
}

TEST_F(FriendInvitationMessageHandlerTest, handleFriendInvitationMessageWithInvalidToken)
{
    const auto validPayloadJson = nlohmann::json{{"token", token}};
    const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
    const auto validMessage =
        common::messages::Message{common::messages::MessageId::SendFriendInvitation, validPayload};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = friendInvitationMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(FriendInvitationMessageHandlerTest, handleFriendInvitationMessageWithErrorWhileFindingFriendEmail)
{
    auto friendEmail = faker::Internet::email();
    const auto validPayloadJson = nlohmann::json{{"data", {{"friend_email", friendEmail}}}, {"token", token}};
    const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
    const auto validMessage =
        common::messages::Message{common::messages::MessageId::SendFriendInvitation, validPayload};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendEmail}))
        .WillOnce(Throw(findFriendEmailError));

    auto responseMessage = friendInvitationMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, findFriendEmailErrorMessageResponse);
}

TEST_F(FriendInvitationMessageHandlerTest, handleFriendInvitationMessageWithErrorWhileHandling)
{
    const auto friendUser = userTestFactory.createDomainUser();
    const auto validPayloadJson =
        nlohmann::json{{"data", {{"friend_email", friendUser->getEmail()}}}, {"token", token}};
    const auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
    const auto validMessage =
        common::messages::Message{common::messages::MessageId::SendFriendInvitation, validPayload};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserByEmailQueryHandler,
                execute(server::application::FindUserByEmailQueryHandlerPayload{friendUser->getEmail()}))
        .WillOnce(Return(server::application::FindUserByEmailQueryHandlerResult{*friendUser}));
    EXPECT_CALL(*createFriendInvitationCommandHandlerMock,
                execute(server::application::CreateFriendInvitationCommandHandlerPayload{userId, friendUser->getId()}))
        .WillOnce(Throw(sendFriendInvitationError));

    auto responseMessage = friendInvitationMessageHandler.handleMessage(validMessage);

    EXPECT_EQ(responseMessage, sendFriendInvitationErrorMessageResponse);
}
