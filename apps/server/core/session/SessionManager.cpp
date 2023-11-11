#include "SessionManager.h"

#include "loguru.hpp"

namespace server::core
{
SessionManager::SessionManager(std::unique_ptr<ConnectionAcceptor> connectionAcceptorInit)
    : connectionAcceptor{std::move(connectionAcceptorInit)}
{
}

void SessionManager::startAcceptingConnections()
{
    connectionAcceptor->startAcceptingConnections([this](const std::shared_ptr<Session>& session)
                                                  { sessions.push_back(session); });
}

void SessionManager::removeInactiveSessions()
{
    LOG_S(INFO) << "Removing inactive sessions";

    for (size_t i = 0; i < sessions.size(); ++i)
    {
        if (not sessions[i]->isActive())
        {
            sessions[i]->close();

            sessions.erase(sessions.begin() + i);

            i--;
        }
    }
}
}
