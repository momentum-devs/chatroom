#include "AddReactionMessageHandler.h"

#include "nlohmann/json.hpp"

namespace server::api
{
AddReactionMessageHandler::AddReactionMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::CreateMessageReactionCommandHandler> createMessageReactionCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      createMessageReactionCommandHandler{std::move(createMessageReactionCommandHandlerInit)}
{
}

common::messages::Message AddReactionMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();
        auto messageId = payloadJson["data"]["messageId"].get<std::string>();
        auto reactionName = payloadJson["data"]["reactionName"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        createMessageReactionCommandHandler->execute(
            {.reactionName = reactionName, .userId = userId, .messageId = messageId});

        return {common::messages::MessageId::AddReactionResponse, common::bytes::Bytes{"[\"ok\"]"}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::AddReactionResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}