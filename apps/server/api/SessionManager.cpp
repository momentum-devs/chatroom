#include "SessionManager.h"

#include "loguru.hpp"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"
#include "messages/MessageSerializerImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionManager::SessionManager(boost::asio::io_context& contextInit, int port)
    : context{contextInit},
      acceptor{context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(port))},
      messageSerializer{std::make_shared<common::messages::MessageSerializerImpl>()}
{
}

void SessionManager::startAcceptingConnections()
{
    LOG_S(INFO) << "Listening for new connection on port: " << acceptor.local_endpoint().port();

    // TODO: implement session creation as a factory
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);
    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);
    std::shared_ptr<Session> newSession =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender));

    acceptor.async_accept(*socket, [this, newSession](const boost::system::error_code& error)
                          { handleConnection(newSession, error); });
}

void SessionManager::handleConnection(std::shared_ptr<Session> newSession, const boost::system::error_code& error)
{
    if (!error)
    {
        newSession->startSession();

        sessions.push_back(newSession);
    }
    else
    {
        LOG_S(ERROR) << "Error: " << error.message();
    }

    startAcceptingConnections();
}
}
