#include "UpdateUserMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
UpdateUserMessageHandler::UpdateUserMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::UpdateUserCommandHandler> updateUserCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, updateUserCommandHandler{std::move(updateUserCommandHandlerInit)}
{
}

common::messages::Message UpdateUserMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto userId = tokenService->getUserIdFromToken(token);

        server::application::UpdateUserCommandHandlerPayload payload{userId};

        if (payloadJson["data"].contains("nickname"))
        {
            payload.nickname = payloadJson["data"]["nickname"].get<std::string>();
        }

        if (payloadJson["data"].contains("password"))
        {
            payload.nickname = payloadJson["data"]["password"].get<std::string>();
        }

        updateUserCommandHandler->execute(payload);

        nlohmann::json responsePayload{{
            "ok",
        }};

        auto message = common::messages::Message{common::messages::MessageId::UpdateUserResponse,
                                                 common::bytes::Bytes{responsePayload.dump()}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::UpdateUserResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}