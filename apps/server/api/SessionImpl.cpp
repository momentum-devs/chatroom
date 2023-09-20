#include "SessionImpl.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace server::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<MessageRouter> messageRouterInit,
                         std::shared_ptr<server::application::TokenService> tokenServiceInit,
                         std::unique_ptr<server::application::LogoutUserCommandHandler> logoutUserCommandHandlerInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      messageRouter{std::move(messageRouterInit)},
      tokenService{std::move(tokenServiceInit)},
      logoutUserCommandHandler{std::move(logoutUserCommandHandlerInit)}
{
}

void SessionImpl::startSession()
{
    startReceivingMessage();
}

void SessionImpl::sendMessage(const common::messages::Message& message)
{
    messageSender->sendMessage(message);
}

void SessionImpl::startReceivingMessage()
{
    LOG_S(INFO) << "Start reading messages";

    messageReader->startReadingMessages([this](const common::messages::Message& message) { handleMessage(message); });
}

void SessionImpl::handleMessage(const common::messages::Message& message)
{
    LOG_S(INFO) << std::format("Received message: (id: {0:}, payload: {1:}), id {0:}", toString(message.id),
                               static_cast<std::string>(message.payload));

    if (message.id == common::messages::MessageId::Logout)
    {
        userId = std::nullopt;
    }

    auto response = messageRouter->route(message);

    messageSender->sendMessage(response);

    if (response.id == common::messages::MessageId::LoginResponse)
    {
        storeCurrentUser(response);
    }
}

bool SessionImpl::isActive()
{
    return true; // TODO: implement keep alive mechanism
}

void SessionImpl::close()
{
    LOG_S(INFO) << "Closing inactive session";

    if (userId)
    {
        logoutUserCommandHandler->execute({userId.value()});
    }
}

void SessionImpl::storeCurrentUser(const common::messages::Message& message)
{
    try
    {
        auto payloadJson = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto token = payloadJson["token"].get<std::string>();

        userId = tokenService->getUserIdFromToken(token);
    }
    catch (const std::exception& e)
    {
        LOG_S(ERROR) << "Error while storing user in session: " << e.what();

        userId = std::nullopt;
    }
}
}
