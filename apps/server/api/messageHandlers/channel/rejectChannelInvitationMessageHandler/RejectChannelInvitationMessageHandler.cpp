#include "RejectChannelInvitationMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
RejectChannelInvitationMessageHandler::RejectChannelInvitationMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::RejectChannelInvitationCommandHandler> rejectChannelInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      rejectChannelInvitationCommandHandler{std::move(rejectChannelInvitationCommandHandlerInit)}
{
}

common::messages::Message
RejectChannelInvitationMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [recipientId] = tokenService->verifyToken(token);

        rejectChannelInvitationCommandHandler->execute({recipientId, channelId});

        LOG_S(INFO) << std::format("Reject invitation to channel {} by user with id {}", channelId, recipientId);

        common::messages::Message responseMessage{common::messages::MessageId::ChangeChannelInvitationResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::ChangeChannelInvitationResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
