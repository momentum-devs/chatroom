#include "LogoutMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
LogoutMessageHandler::LogoutMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::LogoutUserCommandHandler> logoutUserCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, logoutUserCommandHandler{std::move(logoutUserCommandHandlerInit)}
{
}
common::messages::Message LogoutMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        logoutUserCommandHandler->execute({userId});

        return {};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::Error, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
