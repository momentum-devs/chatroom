#include "AcceptFriendRequestMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
AcceptFriendRequestMessageHandler::AcceptFriendRequestMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::AcceptFriendInvitationCommandHandler> acceptFriendInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      acceptFriendInvitationCommandHandler{std::move(acceptFriendInvitationCommandHandlerInit)}
{
}

common::messages::Message
AcceptFriendRequestMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto requestId = payloadJson["data"]["requestId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [recipientId] = tokenService->verifyToken(token);

        acceptFriendInvitationCommandHandler->execute({recipientId, requestId});

        LOG_S(INFO) << std::format("Accept friend invitation with id {} by user with id {}", requestId, recipientId);

        common::messages::Message responseMessage{common::messages::MessageId::ChangeFriendRequestsResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::ChangeFriendRequestsResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
