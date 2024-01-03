#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "FriendInvitationMessageHandler.h"

namespace server::api
{
FriendInvitationMessageHandler::FriendInvitationMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::shared_ptr<application::FindUserByEmailQueryHandler> findUserByEmailQueryHandlerInit,
    std::unique_ptr<application::CreateFriendInvitationCommandHandler> createFriendInvitationCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findUserByEmailQueryHandler{std::move(findUserByEmailQueryHandlerInit)},
      createFriendInvitationCommandHandler{std::move(createFriendInvitationCommandHandlerInit)}
{
}

common::messages::Message FriendInvitationMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto friendEmail = payloadJson["data"]["friend_email"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [senderId] = tokenService->verifyToken(token);

        auto invitedId = findUserByEmailQueryHandler->execute({friendEmail}).user.getId();

        createFriendInvitationCommandHandler->execute({senderId, invitedId});

        LOG_S(INFO) << fmt::format("Sent invitation to friend to user with id {} by user with id {}", invitedId,
                                   senderId);

        common::messages::Message responseMessage{common::messages::MessageId::SendFriendInvitationResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::SendFriendInvitationResponse,
                common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
