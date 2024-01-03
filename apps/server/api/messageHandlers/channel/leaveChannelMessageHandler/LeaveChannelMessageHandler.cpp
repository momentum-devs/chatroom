#include "LeaveChannelMessageHandler.h"

#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
LeaveChannelMessageHandler::LeaveChannelMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::LeaveChannelCommandHandler> leaveChannelCommandHandlerInit)
    : tokenService{std::move(tokenServiceInit)}, leaveChannelCommandHandler{std::move(leaveChannelCommandHandlerInit)}
{
}

common::messages::Message LeaveChannelMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        leaveChannelCommandHandler->execute({userId, channelId});

        LOG_S(INFO) << fmt::format("User with id: {} left the channel with id {}", userId, channelId);

        common::messages::Message responseMessage{common::messages::MessageId::LeftTheChannelResponse,
                                                  common::bytes::Bytes{R"(["ok"])"}};

        return responseMessage;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::LeftTheChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
