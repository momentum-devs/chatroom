#include "RemoveFromFriendsMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
RemoveFromFriendsMessageHandler::RemoveFromFriendsMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::DeleteFriendshipCommandHandler> deleteFriendshipCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      deleteFriendshipCommandHandler{std::move(deleteFriendshipCommandHandlerInit)}
{
}

common::messages::Message RemoveFromFriendsMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto userFriendId = payloadJson["data"]["userFriendId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        deleteFriendshipCommandHandler->execute({userId, userFriendId});

        LOG_S(INFO) << std::format("User with id: {} remove user with id: {} from friends", userId, userFriendId);

        common::messages::Message responseMessage{common::messages::MessageId::RemoveFromFriendsResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::RemoveFromFriendsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
