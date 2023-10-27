#include "GetUserFriendRequestsMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
GetUserFriendRequestsMessageHandler::GetUserFriendRequestsMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::FindReceivedFriendInvitationsQueryHandler>
        findReceivedFriendInvitationsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findReceivedFriendInvitationsQueryHandler{std::move(findReceivedFriendInvitationsQueryHandlerInit)}
{
}

common::messages::Message
GetUserFriendRequestsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        const auto& [friendInvitations] = findReceivedFriendInvitationsQueryHandler->execute({userId});

        nlohmann::json friendRequestsJsonArray = nlohmann::json::array();

        for (const auto& friendInvitation : friendInvitations)
        {
            nlohmann::json friendRequestJson{{"id", friendInvitation.getId()},
                                             {"name", friendInvitation.getSender()->getNickname()}};

            friendRequestsJsonArray.push_back(friendRequestJson);
        }

        nlohmann::json responsePayload{{"data", friendRequestsJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetFriendRequestsResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetFriendRequestsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
