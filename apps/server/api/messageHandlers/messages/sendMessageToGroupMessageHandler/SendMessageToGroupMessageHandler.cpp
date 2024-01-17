#include "SendMessageToGroupMessageHandler.h"

#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
SendMessageToGroupMessageHandler::SendMessageToGroupMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::CreateGroupMessageCommandHandler> createGroupMessageCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      createGroupMessageCommandHandler{std::move(createGroupMessageCommandHandlerInit)}
{
}

common::messages::Message
SendMessageToGroupMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto groupId = payloadJson["data"]["groupId"].get<std::string>();
        auto text = payloadJson["data"]["text"].get<std::string>();

        auto [senderId] = tokenService->verifyToken(token);

        auto [newMessage] = createGroupMessageCommandHandler->execute({text, senderId, groupId});

        nlohmann::json responsePayload;

        responsePayload["data"]["message"] = {
            {"id", newMessage.getId()},
            {"text", newMessage.getContent()},
            {"senderName", newMessage.getSender()->getNickname()},
            {"sentAt", newMessage.getCreatedAt()},
        };

        return {common::messages::MessageId::SendPrivateMessageResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendPrivateMessageResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
