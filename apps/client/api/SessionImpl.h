#pragma once

#include "common/messages/MessageReader.h"
#include "common/messages/MessageSender.h"
#include "messages/MessageSerializer.h"
#include "Session.h"

namespace client::api
{
class SessionImpl: public Session
{
public:
    SessionImpl(std::shared_ptr<common::messages::MessageSerializer> messageSerializer);
    void connect(const std::string& hostName, unsigned short portNumber) override;

private:
    boost::asio::io_context context;

    std::unique_ptr<common::messages::MessageReader> messageReader;
    std::unique_ptr<common::messages::MessageSender> messageSender;
    std::shared_ptr<common::messages::MessageSerializer> messageSerializer;
};
}
