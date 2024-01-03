#include "SendChannelInvitationMessageHandler.h"

#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
SendChannelInvitationMessageHandler::SendChannelInvitationMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::shared_ptr<application::FindUserByEmailQueryHandler> findUserByEmailQueryHandlerInit,
    std::unique_ptr<application::CreateChannelInvitationCommandHandler> createChannelInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findUserByEmailQueryHandler{std::move(findUserByEmailQueryHandlerInit)},
      createChannelInvitationCommandHandler{std::move(createChannelInvitationCommandHandlerInit)}
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

        auto [senderId] = tokenService->verifyToken(token);

        auto invitedId = findUserByEmailQueryHandler->execute({email}).user.getId();

        createChannelInvitationCommandHandler->execute({senderId, invitedId, channelId});

        LOG_S(INFO) << fmt::format("Sent invitation to channel {} to user with id {} by user with id {}", channelId,
                                   invitedId, senderId);

        common::messages::Message responseMessage{common::messages::MessageId::SendChannelInvitationResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendChannelInvitationResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
