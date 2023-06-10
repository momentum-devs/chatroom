#include "MessageHandlerImpl.h"

#include <limits>
#include <nlohmann/json.hpp>
#include <regex>

namespace server::api
{
MessageHandlerImpl::MessageHandlerImpl(
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandlerInit)
    : registerUserCommandHandler{std::move(registerUserCommandHandlerInit)}
{
}

common::messages::Message MessageHandlerImpl::handleMessage(const common::messages::Message& message)
{
    switch (message.id)
    {
    case common::messages::MessageId::Register:
    {
        return handleRegisterMessage(message.payload);
    }
    default:
        return {common::messages::MessageId::Error, {}, {}};
    }
}

const common::bytes::Bytes& MessageHandlerImpl::getToken()
{
    if (not token)
    {
        token = common::bytes::Bytes{};

        for (std::size_t i = 0; i < 40; ++i)
        {
            token->push_back(static_cast<unsigned char>(rand() % std::numeric_limits<unsigned char>::max()));
        }
    }

    return *token;
}

common::messages::Message MessageHandlerImpl::handleRegisterMessage(const common::bytes::Bytes& payload)
{
    auto payloadJson = nlohmann::json::parse(static_cast<std::string>(payload));

    auto email = payloadJson["email"].get<std::string>();

    const std::regex emailRegex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");

    if (not std::regex_match(email, emailRegex))
    {
        nlohmann::json responsePayload{{"error", "wrong email address"}};

        return {common::messages::MessageId::RegisterResponse, nullToken, common::bytes::Bytes{responsePayload.dump()}};
    }

    auto password = payloadJson["password"].get<std::string>();

    try
    {
        registerUserCommandHandler->execute({email, password});
    }
    catch (const std::exception& e)
    {
        nlohmann::json responsePayload{{"error", e.what()}};

        return {common::messages::MessageId::RegisterResponse, nullToken, common::bytes::Bytes{responsePayload.dump()}};
    }

    return {common::messages::MessageId::RegisterResponse, getToken(), common::bytes::Bytes{R"({"ok"})"}};
}
}