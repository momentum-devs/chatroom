#include "GetUserChannelsMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
GetUserChannelsMessageHandler::GetUserChannelsMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandler>
        findChannelsToWhichUserBelongsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findChannelsToWhichUserBelongsQueryHandler{std::move(findChannelsToWhichUserBelongsQueryHandlerInit)}
{
}

common::messages::Message GetUserChannelsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto userId = tokenService->getUserIdFromToken(token);

        const auto& [channels] = findChannelsToWhichUserBelongsQueryHandler->execute({userId});

        nlohmann::json channelsJsonArray = nlohmann::json::array();

        for (const auto& channel : channels)
        {
            nlohmann::json channelJson{{"id", channel.getId()}, {"name", channel.getName()}};

            channelsJsonArray.push_back(channelJson);
        }

        nlohmann::json responsePayload{{"data", channelsJsonArray}};

        auto message = common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                                 common::bytes::Bytes{responsePayload.dump()}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserChannelsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}