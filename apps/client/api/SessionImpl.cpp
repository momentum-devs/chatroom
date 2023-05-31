#include "SessionImpl.h"

#include "messages/Message.h"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"

namespace client::api
{
SessionImpl::SessionImpl(std::shared_ptr<common::messages::MessageSerializer> messageSerializerInit)
    : context{}, messageSerializer(std::move(messageSerializerInit))
{
}

void SessionImpl::connect(const std::string& hostName, unsigned short portNumber)
{
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(hostName), portNumber};

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    socket->connect(endpoint);

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    common::messages::Message message{
        common::messages::MessageId::Register,
        common::messages::nullToken,
        common::bytes::Bytes{"payload"}
    };

    messageSender->sendMessage(message);

    context.run();
}
}