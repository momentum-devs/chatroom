#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "common/messages/MessageSerializer.h"
#include "Session.h"
#include "SessionFactory.h"

namespace server::api
{
class SessionManager
{
public:
    SessionManager(boost::asio::io_context& context, int port, std::unique_ptr<SessionFactory> sessionFactory);
    void startAcceptingConnections();

private:
    void handleConnection(std::shared_ptr<Session> newSession, const boost::system::error_code& error);

    boost::asio::io_context& context;
    boost::asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<Session>> sessions;
    std::unique_ptr<SessionFactory> sessionFactory;
};
}