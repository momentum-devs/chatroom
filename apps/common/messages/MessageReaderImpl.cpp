#include "MessageReaderImpl.h"

#include <boost/beast/core/tcp_stream.hpp>

#include "common/messages/errors/InvalidChecksumError.h"
#include "loguru.hpp"

namespace common::messages
{
MessageReaderImpl::MessageReaderImpl(boost::asio::io_context& contextInit,
                                     std::shared_ptr<boost::asio::ip::tcp::socket> socketInit,
                                     std::shared_ptr<MessageSerializer> messageSerializerInit)
    : context{contextInit},
      socket{std::move(socketInit)},
      messageSerializer{std::move(messageSerializerInit)},
      onReadMessageCallback{nullptr}
{
}

void MessageReaderImpl::startReadingMessages(std::function<void(const common::messages::Message&)> onReadMessageInit)
{
    onReadMessageCallback = onReadMessageInit;

    boost::asio::socket_base::keep_alive option(true);

    socket->set_option(option);

    startReadingNewMessage();
}

void MessageReaderImpl::startReadingNewMessage()
{
    asyncRead(4, [this](boost::system::error_code error, std::size_t bytes) { onReadMessageLength(error, bytes); });
}

void MessageReaderImpl::onReadMessageLength(boost::system::error_code errorCode, std::size_t)
{
    if (errorCode)
    {
        LOG_S(ERROR) << "Error : " << errorCode.message() << std::endl;
        socket->close();
        return;
    }

    const common::bytes::Bytes bytes{std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()};

    const auto bytesToRead = static_cast<u_int32_t>(bytes);

    if (bytesToRead == 0)
    {
        startReadingNewMessage();

        return;
    }

    asyncRead(bytesToRead, [this, bytesToRead](boost::system::error_code error, std::size_t bytes)
              { onReadMessage(error, bytes, bytesToRead); });
}

void MessageReaderImpl::onReadMessage(boost::system::error_code errorCode, std::size_t, std::size_t)
{
    if (errorCode)
    {
        LOG_S(ERROR) << "Error : " << errorCode.message() << std::endl;
        socket->close();
        return;
    }

    const common::bytes::Bytes bytes{std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()};

    try
    {
        auto message = messageSerializer->deserialize(bytes);

        context.post([this, message]() { onReadMessageCallback(message); });
    }
    catch (const std::exception& e)
    {
        LOG_S(ERROR) << e.what();
    }

    startReadingNewMessage();
}

void MessageReaderImpl::asyncRead(std::size_t bytesToRead,
                                  std::function<void(boost::system::error_code, std::size_t)> readHandler)
{
    boost::asio::async_read(*socket, response, boost::asio::transfer_exactly(bytesToRead), readHandler);
}

boost::asio::ip::tcp::socket& MessageReaderImpl::getSocket()
{
    return *socket;
}
}
