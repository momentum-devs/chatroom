#include "RejectFriendRequestMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
RejectFriendRequestMessageHandler::RejectFriendRequestMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::RejectFriendInvitationCommandHandler> rejectFriendInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      rejectFriendInvitationCommandHandler{std::move(rejectFriendInvitationCommandHandlerInit)}
{
}

common::messages::Message
RejectFriendRequestMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["requestId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto recipientId = tokenService->getUserIdFromToken(token);

        rejectFriendInvitationCommandHandler->execute({recipientId, channelId});

        LOG_S(INFO) << std::format("Reject friend request with id {} by user with id {}", channelId, recipientId);

        common::messages::Message message{common::messages::MessageId::ChangeFriendRequestsResponse,
                                          common::bytes::Bytes{R"(["ok"])"}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::ChangeFriendRequestsResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}