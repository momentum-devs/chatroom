#include "SessionImpl.h"

#include "loguru.hpp"

namespace server::api
{
SessionImpl::SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReaderInit,
                         std::unique_ptr<common::messages::MessageSender> messageSenderInit)
    : messageReader{std::move(messageReaderInit)}, messageSender{std::move(messageSenderInit)}
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
    LOG_S(INFO) << "Read message: " << message;
}
}