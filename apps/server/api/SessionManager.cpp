#include "SessionManager.h"

#include "loguru.hpp"

namespace server::api
{
SessionManager::SessionManager(std::unique_ptr<ConnectionAcceptor> connectionAcceptorInit)
    : connectionAcceptor{std::move(connectionAcceptorInit)}
{
}

void SessionManager::startAcceptingConnections()
{
    connectionAcceptor->startAcceptingConnections([this](const std::shared_ptr<Session>& session)
                                                  { handleConnection(session); });
}

void SessionManager::handleConnection(const std::shared_ptr<Session>& session)
{
    sessions.push_back(session);
}
}
