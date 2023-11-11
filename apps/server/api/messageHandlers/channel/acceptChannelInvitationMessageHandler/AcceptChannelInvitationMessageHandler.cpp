#include "AcceptChannelInvitationMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
AcceptChannelInvitationMessageHandler::AcceptChannelInvitationMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::AcceptChannelInvitationCommandHandler> acceptChannelInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      acceptChannelInvitationCommandHandler{std::move(acceptChannelInvitationCommandHandlerInit)}
{
}

common::messages::Message
AcceptChannelInvitationMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [recipientId] = tokenService->verifyToken(token);

        acceptChannelInvitationCommandHandler->execute({recipientId, channelId});

        LOG_S(INFO) << std::format("Accept invitation to channel {} by user with id {}", channelId, recipientId);

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
