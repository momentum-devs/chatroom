#include "LoginMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
LoginMessageHandler::LoginMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, loginUserCommandHandler{std::move(loginUserCommandHandlerInit)}
{
}

common::messages::Message LoginMessageHandler::handleMessage(const common::messages::Message& message) const
{
    auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

    auto email = payloadJson["email"].get<std::string>();

    const std::regex emailRegex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");

    if (not std::regex_match(email, emailRegex))
    {
        nlohmann::json responsePayload{{"error", "wrong email address"}};

        return {common::messages::MessageId::LoginResponse, common::bytes::Bytes{responsePayload.dump()}};
    }

    auto password = payloadJson["password"].get<std::string>();

    application::LoginUserCommandHandlerResult loginUserCommandHandlerResult;

    try
    {
        loginUserCommandHandlerResult = loginUserCommandHandler->execute({email, password});
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::LoginResponse, common::bytes::Bytes{responsePayload.dump()}};
    }

    nlohmann::json responsePayload{{"token", loginUserCommandHandlerResult.token}};

    LOG_S(INFO) << std::format("Logged in user {} with id {}", email,
                               tokenService->getUserIdFromToken(loginUserCommandHandlerResult.token));

    return {common::messages::MessageId::LoginResponse, common::bytes::Bytes{responsePayload.dump()}};
}
}