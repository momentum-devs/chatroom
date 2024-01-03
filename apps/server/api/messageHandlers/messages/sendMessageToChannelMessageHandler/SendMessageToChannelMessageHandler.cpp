#include "SendMessageToChannelMessageHandler.h"

#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
SendMessageToChannelMessageHandler::SendMessageToChannelMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::CreateChannelMessageCommandHandler> createChannelMessageCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      createChannelMessageCommandHandler{std::move(createChannelMessageCommandHandlerInit)}
{
}

common::messages::Message SendMessageToChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto channelId = payloadJson["data"]["channelId"].get<std::string>();
        auto text = payloadJson["data"]["text"].get<std::string>();

        auto [senderId] = tokenService->verifyToken(token);

        auto [newMessage] = createChannelMessageCommandHandler->execute({text, senderId, channelId});

        nlohmann::json responsePayload;

        responsePayload["data"]["message"] = {
            {"id", newMessage.getId()},
            {"text", newMessage.getContent()},
            {"senderName", newMessage.getSender()->getNickname()},
            {"sentAt", newMessage.getCreatedAt()},
        };

        return {common::messages::MessageId::SendChannelMessageResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendChannelMessageResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
