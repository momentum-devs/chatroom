#include "GetChannelMembersMessageHandler.h"

#include "fmt/format.h"
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
GetChannelMembersMessageHandler::GetChannelMembersMessageHandler(
    std::shared_ptr<application::TokenService> tokenServiceInit,
    std::unique_ptr<application::FindUsersBelongingToChannelQueryHandler> findUsersBelongingToChannelQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      findUsersBelongingToChannelQueryHandler{std::move(findUsersBelongingToChannelQueryHandlerInit)}
{
}

common::messages::Message GetChannelMembersMessageHandler::handleMessage(const common::messages::Message& message) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        auto [userId] = tokenService->verifyToken(token);

        auto channelId = payloadJson["data"]["channelId"].get<std::string>();

        const auto& [channelMembers] = findUsersBelongingToChannelQueryHandler->execute({channelId});

        nlohmann::json channelMembersJsonArray = nlohmann::json::array();

        for (const auto& channelMember : channelMembers)
        {
            if (channelMember.getId() == userId)
            {
                continue;
            }

            nlohmann::json channelMemberJson{{"id", channelMember.getId()},
                                             {"name", channelMember.getNickname()},
                                             {"isActive", channelMember.isActive()}};

            channelMembersJsonArray.push_back(channelMemberJson);
        }

        nlohmann::json responsePayload{{"data", channelMembersJsonArray}};

        return common::messages::Message{common::messages::MessageId::GetChannelMembersResponse,
                                         common::bytes::Bytes{responsePayload.dump()}};
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetChannelMembersResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
