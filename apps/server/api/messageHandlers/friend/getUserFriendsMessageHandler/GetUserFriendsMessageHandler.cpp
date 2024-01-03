#include "GetUserFriendsMessageHandler.h"

#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "fmt/format.h"

namespace server::api
{
GetUserFriendsMessageHandler::GetUserFriendsMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindUserFriendshipsQueryHandler> findUserFriendshipsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findUserFriendshipsQueryHandler{std::move(findUserFriendshipsQueryHandlerInit)}
{
}

common::messages::Message GetUserFriendsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        const auto& [friendships] = findUserFriendshipsQueryHandler->execute({userId});

        nlohmann::json friendsJsonArray = nlohmann::json::array();

        for (const auto& friendship : friendships)
        {
            const auto userFriend =
                friendship.getUser()->getId() == userId ? *friendship.getUserFriend() : *friendship.getUser();

            nlohmann::json friendJson{
                {"id", userFriend.getId()}, {"name", userFriend.getNickname()}, {"isActive", userFriend.isActive()}};

            friendsJsonArray.push_back(friendJson);
        }

        nlohmann::json responsePayload{{"data", friendsJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetUserFriendsResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserFriendsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
