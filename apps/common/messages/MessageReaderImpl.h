#pragma once

#include <boost/asio.hpp>

#include "MessageReader.h"
#include "MessageSerializer.h"

namespace common::messages
{
class MessageReaderImpl : public MessageReader
{
public:
    MessageReaderImpl(boost::asio::io_context& context, std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                      std::shared_ptr<MessageSerializer> messageSerializer);
    void startReadingMessages(std::function<void(const common::messages::Message&)> onReadMessage) override;
    boost::asio::ip::tcp::socket& getSocket() override;

private:
    void startReadingNewMessage();
    void onReadMessageLength(boost::system::error_code error, std::size_t bytesTransferred);
    void onReadMessage(boost::system::error_code error, std::size_t bytesTransferred, std::size_t bytesToRead);
    void asyncRead(std::size_t bytesToRead, std::function<void(boost::system::error_code, std::size_t)> readHandler);

    boost::asio::streambuf response;
    boost::asio::io_context& context;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    std::shared_ptr<MessageSerializer> messageSerializer;
    std::function<void(const common::messages::Message&)> onReadMessageCallback;
};
}
