#include "ConnectionAcceptorImpl.h"

#include "loguru.hpp"

namespace server::api
{
ConnectionAcceptorImpl::ConnectionAcceptorImpl(boost::asio::io_context& context, int port,
                                               std::unique_ptr<SessionFactory> sessionFactoryInit)
    : acceptor{context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(port))},
      sessionFactory{std::move(sessionFactoryInit)}
{
}

void ConnectionAcceptorImpl::startAcceptingConnections(
    std::function<void(const std::shared_ptr<Session>&)> newSessionHandlerInit)
{
    LOG_S(INFO) << "Start accepting new connection on port: " << acceptor.local_endpoint().port();

    newSessionHandler = newSessionHandlerInit;

    asyncAccept();
}

void ConnectionAcceptorImpl::asyncAccept()
{
    auto [socket, session] = sessionFactory->create();

    acceptor.async_accept(*socket, [this, session = std::move(session), socket](const boost::system::error_code& error)
                          { onNewConnection(session, socket, error); });
}

void ConnectionAcceptorImpl::onNewConnection(const std::shared_ptr<Session>& newSession,
                                             const std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
                                             const boost::system::error_code& error)
{
    if (!error)
    {
        LOG_S(INFO) << "New connection from: " << socket->remote_endpoint();

        newSession->startSession();

        newSessionHandler(newSession);
    }
    else
    {
        LOG_S(ERROR) << "Error: " << error.message();
    }

    asyncAccept();
}
}