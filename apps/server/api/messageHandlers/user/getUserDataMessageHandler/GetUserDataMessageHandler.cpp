#include "GetUserDataMessageHandler.h"

#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
GetUserDataMessageHandler::GetUserDataMessageHandler(
    std::shared_ptr<application::TokenService> tokenService,
    std::shared_ptr<application::FindUserQueryHandler> findUserQueryHandler)
    : tokenService{std::move(tokenService)}, findUserQueryHandler{std::move(findUserQueryHandler)}
{
}
common::messages::Message GetUserDataMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        const auto& [user] = findUserQueryHandler->execute({userId});

        nlohmann::json userData{
            {"email", user.getEmail()}, {"nickname", user.getNickname()}, {"verified", user.isEmailVerified()}};

        nlohmann::json responsePayload{{"data", userData}};

        auto responseMessage = common::messages::Message{common::messages::MessageId::GetUserDataResponse,
                                                         common::bytes::Bytes{responsePayload.dump()}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserDataResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
