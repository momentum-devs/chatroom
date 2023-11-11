#include "DeleteUserMessageHandler.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
DeleteUserMessageHandler::DeleteUserMessageHandler(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::DeleteUserCommandHandler> deleteUserCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, deleteUserCommandHandler{std::move(deleteUserCommandHandlerInit)}
{
}

common::messages::Message DeleteUserMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        deleteUserCommandHandler->execute({userId});

        nlohmann::json responsePayload{
            "ok",
        };

        auto responseMessage = common::messages::Message{common::messages::MessageId::DeleteUserResponse,
                                                         common::bytes::Bytes{responsePayload.dump()}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::DeleteUserResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
