#include "SendChannelInvitationMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
SendChannelInvitationMessageHandler::SendChannelInvitationMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::shared_ptr<server::application::FindUserQueryHandler> findUserQueryHandlerInit,
    std::shared_ptr<server::application::AddUserToChannelCommandHandler> addUserToChannelCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findUserQueryHandler{std::move(findUserQueryHandlerInit)},
      addUserToChannelCommandHandler{std::move(addUserToChannelCommandHandlerInit)}
{
}

common::messages::Message
SendChannelInvitationMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto email = payloadJson["data"]["email"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto senderId = tokenService->getUserIdFromToken(token);

        auto invitedId = findUserQueryHandler->execute({email}).user.getId();

        addUserToChannelCommandHandler->execute({invitedId, /*senderId,*/ channelId});

        LOG_S(INFO) << std::format("Sent invitation to channel {} to user with id {} by user with id {}", channelId,
                                   invitedId, senderId);

        common::messages::Message message{common::messages::MessageId::SendChannelInvitationResponse,
                                          common::bytes::Bytes{R"(["ok"])"}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendChannelInvitationResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}