#include "GetChannelMessagesMessageHandler.h"

#include "nlohmann/json.hpp"

namespace server::api
{
GetChannelMessagesMessageHandler::GetChannelMessagesMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindChannelMessagesQueryHandler> findChannelMessagesQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findChannelMessagesQueryHandler{std::move(findChannelMessagesQueryHandlerInit)}
{
}

common::messages::Message
GetChannelMessagesMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto channelId = payloadJson["data"]["channelId"].get<std::string>();
        auto limit = payloadJson["data"]["limit"].get<unsigned>();
        auto offset = payloadJson["data"]["offset"].get<unsigned>();

        tokenService->verifyToken(token); // TODO: check if user is in channel

        auto [messages, totalCount] =
            findChannelMessagesQueryHandler->execute({.channelId = channelId, .offset = offset, .limit = limit});

        nlohmann::json responsePayload;

        responsePayload["data"]["messages"] = nlohmann::json::array();
        responsePayload["data"]["totalCount"] = totalCount;

        for (const auto& message : messages)
        {
            responsePayload["data"]["messages"].push_back({
                {"id", message.getId()},
                {"text", message.getContent()},
                {"senderName", message.getSender()->getNickname()},
                {"sentAt", message.getCreatedAt()},
            });
        }

        return {common::messages::MessageId::GetChannelMessagesResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetChannelMessagesResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}