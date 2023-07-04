#include "SessionImpl.h"

#include "loguru.hpp"

namespace server::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<MessageRouter> messageRouterInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      messageRouter{std::move(messageRouterInit)}
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
    LOG_S(INFO) << "Read message with payload: " << static_cast<std::string>(message.payload);

    auto response = messageRouter->route(message);

    messageSender->sendMessage(response);
}
}
