#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "GetUserFriendInvitationsMessageHandler.h"

namespace server::api
{
GetUserFriendInvitationsMessageHandler::GetUserFriendInvitationsMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindReceivedFriendInvitationsQueryHandler>
        findReceivedFriendInvitationsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findReceivedFriendInvitationsQueryHandler{std::move(findReceivedFriendInvitationsQueryHandlerInit)}
{
}

common::messages::Message
GetUserFriendInvitationsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        const auto& [friendInvitations] = findReceivedFriendInvitationsQueryHandler->execute({userId});

        nlohmann::json friendInvitationsJsonArray = nlohmann::json::array();

        for (const auto& friendInvitation : friendInvitations)
        {
            nlohmann::json friendInvitationJson{{"id", friendInvitation.getId()},
                                                {"name", friendInvitation.getSender()->getNickname()}};

            friendInvitationsJsonArray.push_back(friendInvitationJson);
        }

        nlohmann::json responsePayload{{"data", friendInvitationsJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetFriendInvitationsResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetFriendInvitationsResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
