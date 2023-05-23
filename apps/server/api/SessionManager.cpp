#include "SessionManager.h"

#include "loguru.hpp"
#include "SessionImpl.h"

namespace server::api
{
SessionManager::SessionManager(boost::asio::io_context& contextInit, int port)
    : context{contextInit}, acceptor{context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(port))}
{
}

void SessionManager::startAcceptingConnections()
{
    LOG_S(INFO) << "Listening for new connection on port: " << acceptor.local_endpoint().port();

    //TODO: implement session creation as a factory
    std::shared_ptr<Session> newSession = std::make_shared<SessionImpl>(context);

    acceptor.async_accept(newSession->getSocket(), [this, newSession](const boost::system::error_code& error)
                          { handleConnection(newSession, error); });
}

void SessionManager::handleConnection(std::shared_ptr<Session> newSession, const boost::system::error_code& error)
{
    if (!error)
    {
        LOG_S(INFO) << "New connection on port " << newSession->getSocket().remote_endpoint();

        // TODO: implement start new session

        //TODO: store sessions in container
    }
    else
    {
        LOG_S(ERROR) << "Error: " << error.message();

        // TODO: implement handle error
    }

    startAcceptingConnections();
}
}
