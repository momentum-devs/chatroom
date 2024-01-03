#include "GetUserFriendsMessageHandler.h"

#include <gtest/gtest.h>
#include <regex>

#include "server/application/queryHandlers/friend/findUserFriendshipsQueryHandler/FindUserFriendshipsQueryHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include "faker-cxx/Date.h"
#include "faker-cxx/Image.h"
#include "faker-cxx/String.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include "server/tests/factories/friendshipTestFactory/FriendshipTestFactory.h"

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

auto friendId1 = "id1";
auto friendName1 = "friendName1";
auto friendId2 = "id2";
auto friendName2 = "friendName2";

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
    const auto email = "email";
    const auto password = "password";
    const auto emailVerified = true;
    const auto verificationCode = faker::String::numeric(6);
    const auto createdAt = faker::Date::pastDate();
    const auto updatedAt = faker::Date::recentDate();
    const auto avatarUrl = faker::Image::imageUrl();

    const auto user1 =
        std::make_shared<server::domain::User>(friendId1, email, password, friendName1, false, emailVerified,
                                               verificationCode, createdAt, updatedAt, avatarUrl);
    const auto user2 =
        std::make_shared<server::domain::User>(friendId2, email, password, friendName2, true, emailVerified,
                                               verificationCode, createdAt, updatedAt, avatarUrl);

    const auto groupId = faker::String::uuid();

    const auto group = std::make_shared<server::domain::Group>(groupId, createdAt);

    const auto verifyTokenResult = server::application::VerifyTokenResult{friendId1};

    const auto friendship = friendshipTestFactory.createDomainFriendship(user1, user2, group);

    EXPECT_CALL(*tokenServiceMock, verifyToken(token)).WillOnce(Return(verifyTokenResult));
    EXPECT_CALL(*findUserFriendshipsQueryHandlerMock,
                execute(server::application::FindUserFriendshipsQueryHandlerPayload{friendId1}))
        .WillOnce(Return(server::application::FindUserFriendshipsQueryHandlerResult{{*friendship}}));

    auto responseMessage = getUserFriendsMessageHandler.handleMessage(message);

    auto expectedResponsePayloadJson = nlohmann::json{
        {"data", nlohmann::json::array({{{"id", friendId2}, {"name", friendName2}, {"isActive", true}}})}};

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
