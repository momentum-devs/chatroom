#include "SessionManager.h"

#include "loguru.hpp"
#include "messages/MessageReaderImpl.h"
#include "messages/MessageSenderImpl.h"
#include "messages/MessageSerializerImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionManager::SessionManager(boost::asio::io_context& contextInit, int port,
                               std::unique_ptr<SessionFactory> sessionFactoryInit)
    : context{contextInit},
      acceptor{context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(port))},
      sessionFactory{std::move(sessionFactoryInit)}
{
}

void SessionManager::startAcceptingConnections()
{
    LOG_S(INFO) << "Listening for new connection on port: " << acceptor.local_endpoint().port();

    auto session = sessionFactory->create();

    acceptor.async_accept(session->getSocket(), [this, session](const boost::system::error_code& error)
                          { handleConnection(session, error); });
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
