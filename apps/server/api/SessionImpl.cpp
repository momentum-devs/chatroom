#include "SessionImpl.h"

#include <glog/logging.h>

namespace server::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit,
                         std::unique_ptr<MessageHandler> messageHandlerInit)
    : messageReader{std::move(messageReaderInit)},
      messageSender{std::move(messageSenderInit)},
      messageHandler{std::move(messageHandlerInit)}
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
    VLOG(0) << "Start reading messages";

    messageReader->startReadingMessages([this](const common::messages::Message& message) { handleMessage(message); });
}

void SessionImpl::handleMessage(const common::messages::Message& message)
{
    VLOG(0) << "Read message with payload: " << static_cast<std::string>(message.payload);

    auto response = messageHandler->handleMessage(message);

    messageSender->sendMessage(response);
}
}
