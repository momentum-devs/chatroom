#include "GetUserFriendInvitationsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/friendInvitationTestFactory/FriendInvitationTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetFriendInvitations, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getFriendInvitationsError("getFriendInvitationsError");
auto getFriendInvitationsErrorMessageResponse =
    common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                              common::bytes::Bytes{R"({"error":"getFriendInvitationsError"})"}};
}

class GetUserFriendInvitationsMessageHandlerTest : public Test
{
public:
    server::tests::UserTestFactory userTestFactory;
    server::tests::FriendInvitationTestFactory friendInvitationTestFactory;

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindReceivedFriendInvitationsQueryHandlerMock>
        findReceivedFriendInvitationsQueryHandlerMockInit =
            std::make_unique<StrictMock<server::application::FindReceivedFriendInvitationsQueryHandlerMock>>();
    server::application::FindReceivedFriendInvitationsQueryHandlerMock* findReceivedFriendInvitationsQueryHandlerMock =
        findReceivedFriendInvitationsQueryHandlerMockInit.get();

    GetUserFriendInvitationsMessageHandler getUserFriendInvitationsMessageHandler{
        tokenServiceMock, std::move(findReceivedFriendInvitationsQueryHandlerMockInit)};
};

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleValidGetUserFriendInvitationsMessageWithNoFriendInvitations)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindReceivedFriendInvitationsQueryHandlerResult{}));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleValidGetUserFriendInvitationsMessageWithFewFriendInvitations)
{
    const auto user = userTestFactory.createDomainUser();
    const auto sender1 = userTestFactory.createDomainUser();
    const auto sender2 = userTestFactory.createDomainUser();

    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    const auto friendInvitation1 = friendInvitationTestFactory.createDomainFriendInvitation(sender1, user);
    const auto friendInvitation2 = friendInvitationTestFactory.createDomainFriendInvitation(sender2, user);

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindReceivedFriendInvitationsQueryHandlerResult{
            {*friendInvitation1, *friendInvitation2}}));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    auto fewFriendInvitationsResponsePayloadJson = nlohmann::json{
        {"data", nlohmann::json::array({{{"id", friendInvitation1->getId()}, {"name", sender1->getNickname()}},
                                        {{"id", friendInvitation2->getId()}, {"name", sender2->getNickname()}}})}};

    auto fewFriendInvitationsMessageResponse =
        common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                                  common::bytes::Bytes{fewFriendInvitationsResponsePayloadJson.dump()}};

    EXPECT_EQ(responseMessage, fewFriendInvitationsMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleGetUserFriendInvitationsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserFriendInvitationsMessageHandlerTest, handleGetUserFriendInvitationsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findReceivedFriendInvitationsQueryHandlerMock,
                execute(server::application::FindReceivedFriendInvitationsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getFriendInvitationsError));

    auto responseMessage = getUserFriendInvitationsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getFriendInvitationsErrorMessageResponse);
}
