#pragma once

#include "ConnectionAcceptor.h"
#include "SessionFactory.h"

namespace server::core
{
class ConnectionAcceptorImpl : public ConnectionAcceptor
{
public:
    ConnectionAcceptorImpl(boost::asio::io_context& context, int port, std::unique_ptr<SessionFactory> sessionFactory);

    void startAcceptingConnections(std::function<void(const std::shared_ptr<Session>&)> newSessionHandler) override;

private:
    void asyncAccept();
    void onNewConnection(const std::shared_ptr<Session>& newSession,
                         const std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
                         const boost::system::error_code& error);

    boost::asio::ip::tcp::acceptor acceptor;
    std::function<void(const std::shared_ptr<Session>&)> newSessionHandler;
    std::unique_ptr<SessionFactory> sessionFactory;
};
}
