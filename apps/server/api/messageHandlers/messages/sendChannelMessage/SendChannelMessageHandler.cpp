#include "SendChannelMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
SendChannelMessageHandler::SendChannelMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::CreateChannelMessageCommandHandler> createChannelMessageCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      createChannelMessageCommandHandler{std::move(createChannelMessageCommandHandlerInit)}
{
}

common::messages::Message SendChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto channelId = payloadJson["data"]["channelId"].get<std::string>();
        auto text = payloadJson["data"]["text"].get<std::string>();

        auto [senderId] = tokenService->verifyToken(token);

        createChannelMessageCommandHandler->execute({text, senderId, channelId});

        return {common::messages::MessageId::SendChannelMessageResponse, common::bytes::Bytes{R"(["ok"])"}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendChannelMessageResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}