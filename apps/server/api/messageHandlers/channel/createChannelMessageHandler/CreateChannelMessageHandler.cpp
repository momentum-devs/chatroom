#include "CreateChannelMessageHandler.h"

#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "fmt/format.h"

namespace server::api
{
CreateChannelMessageHandler::CreateChannelMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::CreateChannelCommandHandler> createChannelCommandHandlerInit)
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

        auto [creatorId] = tokenService->verifyToken(token);

        createChannelCommandHandler->execute({channelName, creatorId});

        LOG_S(INFO) << fmt::format("Created channel {} by user with id {}", channelName, creatorId);

        return {common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{R"(["ok"])"}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
