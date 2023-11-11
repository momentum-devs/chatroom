#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "common/messages/MessageSerializer.h"
#include "ConnectionAcceptor.h"
#include "Session.h"
#include "SessionFactory.h"

namespace server::core
{
class SessionManager
{
public:
    SessionManager(std::unique_ptr<ConnectionAcceptor> connectionAcceptor);

    void startAcceptingConnections();
    void removeInactiveSessions();

private:
    std::vector<std::shared_ptr<Session>> sessions;
    std::unique_ptr<ConnectionAcceptor> connectionAcceptor;
};
}
