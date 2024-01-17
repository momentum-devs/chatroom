#include "GetUserFriendsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findUserFriendshipsQueryHandler/FindUserFriendshipsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/String.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/friendshipTestFactory/FriendshipTestFactory.h"
#include "server/tests/factories/groupTestFactory/GroupTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server::api;

namespace
{
auto token = "token";
auto validPayloadJson = nlohmann::json{{"token", token}};
auto validPayload = common::bytes::Bytes{validPayloadJson.dump()};
auto message = common::messages::Message{common::messages::MessageId::GetUserFriends, validPayload};

auto noFriendInvitationResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array()}};
auto noFriendInvitationMessageResponse =
    common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                              common::bytes::Bytes{noFriendInvitationResponsePayloadJson.dump()}};

std::runtime_error invalidToken("invalidToken");
auto invalidTokenMessageResponse = common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                                                             common::bytes::Bytes{R"({"error":"invalidToken"})"}};

std::runtime_error getUserChannelsError("getUserChannelsError");
auto getUserChannelsErrorMessageResponse = common::messages::Message{
    common::messages::MessageId::GetUserFriendsResponse, common::bytes::Bytes{R"({"error":"getUserChannelsError"})"}};
}

class GetUserFriendsMessageHandlerTest : public Test
{
public:
    server::tests::FriendshipTestFactory friendshipTestFactory;
    server::tests::UserTestFactory userTestFactory;
    server::tests::GroupTestFactory groupTestFactory;

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::FindUserFriendshipsQueryHandlerMock> findUserFriendshipsQueryHandlerMockInit =
        std::make_unique<StrictMock<server::application::FindUserFriendshipsQueryHandlerMock>>();
    server::application::FindUserFriendshipsQueryHandlerMock* findUserFriendshipsQueryHandlerMock =
        findUserFriendshipsQueryHandlerMockInit.get();

    GetUserFriendsMessageHandler getUserFriendsMessageHandler{tokenServiceMock,
                                                              std::move(findUserFriendshipsQueryHandlerMockInit)};
};

TEST_F(GetUserFriendsMessageHandlerTest, handleValidGetUserFriendsMessageWithNoFriends)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendshipsQueryHandlerMock,
                execute(server::application::FindUserFriendshipsQueryHandlerPayload{userId}))
        .WillOnce(Return(server::application::FindUserFriendshipsQueryHandlerResult{}));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, noFriendInvitationMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleValidGetUserFriendsMessageWithFewFriends)
{

    const auto user = userTestFactory.createDomainUser();
    const auto friend1 = userTestFactory.createDomainUser();
    const auto friend2 = userTestFactory.createDomainUser();
    const auto group1 = groupTestFactory.createDomainGroup();
    const auto group2 = groupTestFactory.createDomainGroup();
    const auto friendship1 = friendshipTestFactory.createDomainFriendship(user, friend1, group1);
    const auto friendship2 = friendshipTestFactory.createDomainFriendship(user, friend2, group2);
    const auto verifyTokenResult = server::application::VerifyTokenResult{user->getId()};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendshipsQueryHandlerMock,
                execute(server::application::FindUserFriendshipsQueryHandlerPayload{user->getId()}))
        .WillOnce(Return(server::application::FindUserFriendshipsQueryHandlerResult{{*friendship1, *friendship2}}));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    auto expectedResponsePayloadJson = nlohmann::json{{"data", nlohmann::json::array({
                                                                   {{"id", friend1->getId()},
                                                                    {"name", friend1->getNickname()},
                                                                    {"isActive", friend1->isActive()},
                                                                    {"groupId", group1->getId()}},
                                                                   {{"id", friend2->getId()},
                                                                    {"name", friend2->getNickname()},
                                                                    {"isActive", friend2->isActive()},
                                                                    {"groupId", group2->getId()}},
                                                               })}};

    auto expectedMessageResponse = common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                                                             common::bytes::Bytes{expectedResponsePayloadJson.dump()}};

    EXPECT_EQ(responseMessage, expectedMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleGetUserFriendsMessageWithInvalidToken)
{
    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Throw(invalidToken));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, invalidTokenMessageResponse);
}

TEST_F(GetUserFriendsMessageHandlerTest, handleGetUserFriendsMessageWithErrorWhileHandling)
{
    const auto userId = faker::String::uuid();

    const auto verifyTokenResult = server::application::VerifyTokenResult{userId};

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendshipsQueryHandlerMock,
                execute(server::application::FindUserFriendshipsQueryHandlerPayload{userId}))
        .WillOnce(Throw(getUserChannelsError));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    EXPECT_EQ(responseMessage, getUserChannelsErrorMessageResponse);
}
