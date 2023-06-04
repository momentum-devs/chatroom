#include "SessionImpl.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace server::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<server::application::CreateUserCommandHandler> createUserCommandHandlerInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      createUserCommandHandler{std::move(createUserCommandHandlerInit)}
{
}

void SessionImpl::startSession()
{
    startReceivingMessage();
}

void SessionImpl::startReceivingMessage()
{
    LOG_S(INFO) << "Start reading messages";

    messageReader->startReadingMessages([this](const common::messages::Message& message) { handleMessage(message); });
}

void SessionImpl::handleMessage(const common::messages::Message& message)
{
    LOG_S(INFO) << "Read message with payload: " << static_cast<std::string>(message.payload);

    // TODO: create message handler

    if (message.id == common::messages::MessageId::Register)
    {
        nlohmann::json payload = nlohmann::json::parse(static_cast<std::string>(message.payload));

        auto email = payload["email"].get<std::string>();

        auto password = payload["password"].get<std::string>();

        createUserCommandHandler->execute({email, password});

        // TODO: send response
    }
}

boost::asio::ip::tcp::socket& SessionImpl::getSocket()
{
    return messageReader->getSocket();
}
}