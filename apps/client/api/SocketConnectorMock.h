#pragma once

#include <gmock/gmock.h>

#include "SocketConnector.h"

namespace client::api
{
class SocketConnectorMock : public SocketConnector
{
public:
    MOCK_METHOD(void, connect, (const ConnectorPayload&), (override));
};
}
