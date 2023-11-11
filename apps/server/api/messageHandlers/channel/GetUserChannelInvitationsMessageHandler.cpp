#include "GetUserChannelInvitationsMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
GetUserChannelInvitationsMessageHandler::GetUserChannelInvitationsMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindReceivedChannelInvitationsQueryHandler>
        findReceivedChannelInvitationsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findReceivedChannelInvitationsQueryHandler{std::move(findReceivedChannelInvitationsQueryHandlerInit)}
{
}

common::messages::Message
GetUserChannelInvitationsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        const auto& [channels] = findReceivedChannelInvitationsQueryHandler->execute({userId});

        nlohmann::json channelsJsonArray = nlohmann::json::array();

        for (const auto& channel : channels)
        {
            nlohmann::json channelInvitationJson{{"id", channel.getId()}, {"name", channel.getChannel()->getName()}};

            channelsJsonArray.push_back(channelInvitationJson);
        }

        nlohmann::json responsePayload{{"data", channelsJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetUserChannelInvitationsResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserChannelInvitationsResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
