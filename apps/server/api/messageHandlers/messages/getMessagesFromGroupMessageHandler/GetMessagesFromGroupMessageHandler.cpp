#include "GetMessagesFromGroupMessageHandler.h"

#include "nlohmann/json.hpp"

namespace server::api
{
GetMessagesFromGroupMessageHandler::GetMessagesFromGroupMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindGroupMessagesQueryHandler> findGroupMessagesQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findGroupMessagesQueryHandler{std::move(findGroupMessagesQueryHandlerInit)}
{
}

common::messages::Message
GetMessagesFromGroupMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto groupId = payloadJson["data"]["groupId"].get<std::string>();
        auto limit = payloadJson["data"]["limit"].get<unsigned>();
        auto offset = payloadJson["data"]["offset"].get<unsigned>();

        tokenService->verifyToken(token);

        auto [messages, totalCount] =
            findGroupMessagesQueryHandler->execute({.groupId = groupId, .offset = offset, .limit = limit});

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

        return {common::messages::MessageId::GetPrivateMessagesResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetPrivateMessagesResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}