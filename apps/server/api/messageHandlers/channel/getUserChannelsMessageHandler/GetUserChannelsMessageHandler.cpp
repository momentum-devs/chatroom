#include "GetUserChannelsMessageHandler.h"

#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "fmt/format.h"

namespace server::api
{
GetUserChannelsMessageHandler::GetUserChannelsMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindChannelsToWhichUserBelongsQueryHandler>
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

        auto [userId] = tokenService->verifyToken(token);

        const auto& [channels] = findChannelsToWhichUserBelongsQueryHandler->execute({userId});

        nlohmann::json channelsJsonArray = nlohmann::json::array();

        for (const auto& channel : channels)
        {
            bool isOwner = channel.getCreator()->getId() == userId;

            nlohmann::json channelJson{{"id", channel.getId()}, {"name", channel.getName()}, {"isOwner", isOwner}};

            channelsJsonArray.push_back(channelJson);
        }

        nlohmann::json responsePayload{{"data", channelsJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserChannelsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
