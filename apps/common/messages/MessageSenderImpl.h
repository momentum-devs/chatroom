#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "MessageSender.h"
#include "MessageSerializer.h"

namespace common::messages
{
class MessageSenderImpl: public MessageSender
{
public:
    MessageSenderImpl(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<MessageSerializer> messageSerializer);
    void sendMessage(const Message& message) override;

private:
    void asyncWrite(boost::asio::const_buffer writeBuffer, std::function<void(boost::system::error_code, std::size_t)> writeHandler);

    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    std::shared_ptr<MessageSerializer> messageSerializer;
};
}
