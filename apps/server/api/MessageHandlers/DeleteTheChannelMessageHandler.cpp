#include "DeleteTheChannelMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
DeleteTheChannelMessageHandler::DeleteTheChannelMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::DeleteChannelCommandHandler> deleteChannelCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, deleteChannelCommandHandler{std::move(deleteChannelCommandHandlerInit)}
{
}

common::messages::Message DeleteTheChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        deleteChannelCommandHandler->execute({channelId, userId});

        LOG_S(INFO) << std::format("User with id: {} deleted channel with id {}", userId, channelId);

        common::messages::Message responseMessage{common::messages::MessageId::DeleteTheChannelResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::DeleteTheChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
