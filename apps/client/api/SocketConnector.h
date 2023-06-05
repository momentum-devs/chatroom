#pragma once

#include <string>

#include "ConnectorPayload.h"

namespace client::api
{
class SocketConnector
{
public:
    virtual ~SocketConnector() = default;

    virtual void connect(const ConnectorPayload& connectorPayload) = 0;
};
}