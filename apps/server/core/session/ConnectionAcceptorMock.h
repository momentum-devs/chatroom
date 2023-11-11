#pragma once

#include <gmock/gmock.h>

#include "ConnectionAcceptor.h"

namespace server::core
{
class ConnectionAcceptorMock : public ConnectionAcceptor
{
public:
    MOCK_METHOD(void, startAcceptingConnections, (std::function<void(const std::shared_ptr<Session>&)>), (override));
};
}
