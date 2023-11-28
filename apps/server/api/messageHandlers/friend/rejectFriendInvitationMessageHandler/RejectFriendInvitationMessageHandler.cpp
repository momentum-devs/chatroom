#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "RejectFriendInvitationMessageHandler.h"

namespace server::api
{
RejectFriendInvitationMessageHandler::RejectFriendInvitationMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::RejectFriendInvitationCommandHandler> rejectFriendInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      rejectFriendInvitationCommandHandler{std::move(rejectFriendInvitationCommandHandlerInit)}
{
}

common::messages::Message
RejectFriendInvitationMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["requestId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [recipientId] = tokenService->verifyToken(token);

        rejectFriendInvitationCommandHandler->execute({recipientId, channelId});

        LOG_S(INFO) << std::format("Reject friend request with id {} by user with id {}", channelId, recipientId);

        common::messages::Message responseMessage{common::messages::MessageId::ChangeFriendInvitationsResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::ChangeFriendInvitationsResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
