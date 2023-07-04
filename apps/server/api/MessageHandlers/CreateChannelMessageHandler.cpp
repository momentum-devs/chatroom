#include "CreateChannelMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
CreateChannelMessageHandler::CreateChannelMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::CreateChannelCommandHandler> createChannelCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, createChannelCommandHandler{std::move(createChannelCommandHandlerInit)}
{
}

common::messages::Message CreateChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelName = payloadJson["data"]["channelName"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto creatorId = tokenService->getUserIdFromToken(token);

        createChannelCommandHandler->execute({channelName, creatorId});

        LOG_S(INFO) << std::format("Created channel {} by user with id {}", channelName, creatorId);

        common::messages::Message message{common::messages::MessageId::CreateChannelResponse,
                                          common::bytes::Bytes{R"(["ok"])"}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}