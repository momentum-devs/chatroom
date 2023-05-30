#pragma once

#include "common/messages/MessageReader.h"
#include "common/messages/MessageSender.h"
#include "Session.h"

namespace server::api
{
class SessionImpl final : public Session
{
public:
    SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReader,
                std::unique_ptr<common::messages::MessageSender> messageSender);
    void startSession() override;

private:
    void startReceivingMessage();
    void handleMessage(const common::messages::Message& message);

    std::unique_ptr<common::messages::MessageReader> messageReader;
    std::unique_ptr<common::messages::MessageSender> messageSender;
};
}