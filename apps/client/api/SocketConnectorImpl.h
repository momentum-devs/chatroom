#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "SocketConnector.h"

namespace client::api
{
class SocketConnectorImpl : public SocketConnector
{
public:
    SocketConnectorImpl(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    void connect(const ConnectorPayload& connectorPayload) override;

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
};
}
