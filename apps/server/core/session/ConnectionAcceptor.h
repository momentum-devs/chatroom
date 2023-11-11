#pragma once

#include <memory>

#include "Session.h"

namespace server::core
{
class ConnectionAcceptor
{
public:
    virtual ~ConnectionAcceptor() = default;

    virtual void startAcceptingConnections(std::function<void(const std::shared_ptr<Session>&)> newSessionHandler) = 0;
};
}
