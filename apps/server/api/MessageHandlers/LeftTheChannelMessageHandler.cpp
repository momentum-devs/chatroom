#include "LeftTheChannelMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
LeftTheChannelMessageHandler::LeftTheChannelMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::RemoveUserFromChannelCommandHandler> removeUserFromChannelCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      removeUserFromChannelCommandHandler{std::move(removeUserFromChannelCommandHandlerInit)}
{
}

common::messages::Message LeftTheChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto userId = tokenService->getUserIdFromToken(token);

        removeUserFromChannelCommandHandler->execute({userId, channelId});

        LOG_S(INFO) << std::format("Removed user with id: {} from channel with id {}", userId, channelId);

        common::messages::Message message{common::messages::MessageId::LeftTheChannelResponse,
                                          common::bytes::Bytes{R"(["ok"])"}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::LeftTheChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}