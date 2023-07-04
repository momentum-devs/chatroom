#include "MessageHandlerImpl.h"

#include <format>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
MessageHandlerImpl::MessageHandlerImpl(
    std::shared_ptr<server::application::TokenService> tokenServiceInit,
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandlerInit,
    std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandlerInit,
    std::unique_ptr<server::application::CreateChannelCommandHandler> createChannelCommandHandlerInit,
    std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandler>
        findChannelsToWhichUserBelongsQueryHandlerInit)
    : tokenService{std::move(tokenServiceInit)},
      registerUserCommandHandler{std::move(registerUserCommandHandlerInit)},
      loginUserCommandHandler{std::move(loginUserCommandHandlerInit)},
      createChannelCommandHandler{std::move(createChannelCommandHandlerInit)},
      findChannelsToWhichUserBelongsQueryHandler{std::move(findChannelsToWhichUserBelongsQueryHandlerInit)}
{
}

common::messages::Message MessageHandlerImpl::handleMessage(const common::messages::Message& message)
{
    switch (message.id)
    {
    case common::messages::MessageId::Register:
        return handleRegisterMessage(message.payload);
    case common::messages::MessageId::Login:
        return handleLoginMessage(message.payload);
    case common::messages::MessageId::CreateChannel:
        return handleCreateChannelRequest(message.payload);
    case common::messages::MessageId::GetUserChannels:
        return handleGetUserChannelsRequest(message.payload);
    default:
        return {common::messages::MessageId::Error, {}};
    }
}

common::messages::Message MessageHandlerImpl::handleRegisterMessage(const common::bytes::Bytes& payload) const
{
    auto payloadJson = nlohmann::json::parse(static_cast<std::string>(payload));

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

common::messages::Message MessageHandlerImpl::handleLoginMessage(const common::bytes::Bytes& payload) const
{
    auto payloadJson = nlohmann::json::parse(static_cast<std::string>(payload));

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

common::messages::Message MessageHandlerImpl::handleCreateChannelRequest(const common::bytes::Bytes& payload) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(payload));

        auto channelName = payloadJson["data"]["channelName"].get<std::string>();

        auto token = payloadJson["token"].get<std::string>();

        auto creatorId = tokenService->getUserIdFromToken(token);

        createChannelCommandHandler->execute({channelName, creatorId});

        LOG_S(INFO) << std::format("Created channel {} by user with id {}", channelName, creatorId);

        common::messages::Message message{common::messages::MessageId::CreateChannelResponse,
                                          common::bytes::Bytes{R"(["ok"])"}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::CreateChannelResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}

common::messages::Message MessageHandlerImpl::handleGetUserChannelsRequest(const common::bytes::Bytes& payload) const
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(payload));

        auto token = payloadJson["token"].get<std::string>();

        auto userId = tokenService->getUserIdFromToken(token);

        const auto& [channels] = findChannelsToWhichUserBelongsQueryHandler->execute({userId});

        nlohmann::json channelsJsonArray = nlohmann::json::array();

        for (const auto& channel : channels)
        {
            nlohmann::json channelJson{{"id", channel.getId()}, {"name", channel.getName()}};

            channelsJsonArray.push_back(channelJson);
        }

        nlohmann::json responsePayload{{"data", channelsJsonArray}};

        auto message = common::messages::Message{common::messages::MessageId::GetUserChannelsResponse,
                                                 common::bytes::Bytes{responsePayload.dump()}};

        return message;
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::GetUserChannelsResponse, common::bytes::Bytes{responsePayload.dump()}};
    }
}
}
