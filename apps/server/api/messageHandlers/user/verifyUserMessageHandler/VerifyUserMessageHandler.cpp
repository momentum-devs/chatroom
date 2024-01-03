#include "VerifyUserMessageHandler.h"

#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "fmt/format.h"

namespace server::api
{
VerifyUserMessageHandler::VerifyUserMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::VerifyUserEmailCommandHandler> verifyUserEmailCommandHandlerInit,
    std::shared_ptr<application::FindUserQueryHandler> findUserQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      verifyUserEmailCommandHandler{std::move(verifyUserEmailCommandHandlerInit)},
      findUserQueryHandler{std::move(findUserQueryHandlerInit)}
{
}
common::messages::Message VerifyUserMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        server::application::VerifyUserEmailCommandHandlerPayload payload;

        payload.email = findUserQueryHandler->execute({userId}).user.getEmail();

        if (payloadJson["data"].contains("verificationCode"))
        {
            payload.verificationCode = payloadJson["data"]["verificationCode"].get<std::string>();
        }

        auto [verified] = verifyUserEmailCommandHandler->execute(payload);

        if (not verified)
        {
            throw std::runtime_error("invalid verification code");
        }

        nlohmann::json responsePayload{
            "ok",
        };

        auto responseMessage = common::messages::Message{common::messages::MessageId::VerifyUserResponse,
                                                         common::bytes::Bytes{responsePayload.dump()}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::VerifyUserResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
