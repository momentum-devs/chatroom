#include "SessionImpl.h"

#include "messages/Message.h"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"

namespace client::api
{
SessionImpl::SessionImpl(boost::asio::io_context& contextInit, std::shared_ptr<common::messages::MessageSerializer> messageSerializerInit)
    : context{contextInit}, messageSerializer(std::move(messageSerializerInit))
{
}

void SessionImpl::connect(const std::string& hostName, unsigned short portNumber)
{
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(hostName), portNumber};

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    socket->connect(endpoint);

    messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);
}

void SessionImpl::sendMessage(const common::messages::Message& message)
{
    messageSender->sendMessage(message);
}
}