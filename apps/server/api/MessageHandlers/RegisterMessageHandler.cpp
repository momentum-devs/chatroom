#include "RegisterMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
RegisterMessageHandler::RegisterMessageHandler(
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandlerInit)
    : registerUserCommandHandler{std::move(registerUserCommandHandlerInit)}
{
}

common::messages::Message RegisterMessageHandler::handleMessage(const common::messages::Message& message) const
{
    auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

    auto email = payloadJson["email"].get<std::string>();

    const std::regex emailRegex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");

    if (not std::regex_match(email, emailRegex))
    {
        nlohmann::json responsePayload{{"error", "wrong email address"}};

        return {common::messages::MessageId::RegisterResponse, common::bytes::Bytes{responsePayload.dump()}};
    }

    auto password = payloadJson["password"].get<std::string>();

    std::optional<server::application::RegisterUserCommandHandlerResult> registerUserCommandHandlerResult;

    try
    {
        registerUserCommandHandlerResult = registerUserCommandHandler->execute({email, password});
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::RegisterResponse, common::bytes::Bytes{responsePayload.dump()}};
    }

    LOG_S(INFO) << std::format("Register user {} with id {}", email, registerUserCommandHandlerResult->user.getId());

    return {common::messages::MessageId::RegisterResponse, common::bytes::Bytes{R"(["ok"])"}};
}
}