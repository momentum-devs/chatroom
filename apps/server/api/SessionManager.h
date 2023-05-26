#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "Session.h"

namespace server::api
{
class SessionManager
{
public:
    SessionManager(boost::asio::io_context& contextInit, int port);
    void startAcceptingConnections();

private:
    void handleConnection(std::shared_ptr<Session> newSession, const boost::system::error_code& error);

    boost::asio::io_context& context;
    boost::asio::ip::tcp::acceptor acceptor;
};
}