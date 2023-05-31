#include "MessageSenderImpl.h"

#include "loguru.hpp"

namespace common::messages
{
MessageSenderImpl::MessageSenderImpl(std::shared_ptr<boost::asio::ip::tcp::socket> socketInit,
                                     std::shared_ptr<MessageSerializer> messageSerializerInit):
    socket{std::move(socketInit)}, messageSerializer{std::move(messageSerializerInit)}
{
}

void MessageSenderImpl::sendMessage(const Message& message)
{
    auto serializedMessage = messageSerializer->serialize(message);

    auto serializedMessageWithLength = 
                    bytes::Bytes(static_cast<u_int32_t>(serializedMessage.size()))
                    + serializedMessage;
    
    asyncWrite(boost::asio::buffer(serializedMessageWithLength),
               [](boost::system::error_code, std::size_t bytesTransferred)
               {
                    LOG_S(INFO) << "Sent message size: " << bytesTransferred << " bytes";
               }
    );
}

void MessageSenderImpl::asyncWrite(boost::asio::const_buffer writeBuffer,
                                   std::function<void(boost::system::error_code, std::size_t)> writeHandler)
{
    boost::asio::async_write(*socket, writeBuffer, writeHandler);
}
}