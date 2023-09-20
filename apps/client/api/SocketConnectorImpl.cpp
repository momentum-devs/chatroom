#include "SocketConnectorImpl.h"

namespace client::api
{
SocketConnectorImpl::SocketConnectorImpl(std::shared_ptr<boost::asio::ip::tcp::socket> socketInit)
    : socket{std::move(socketInit)}
{
}

void SocketConnectorImpl::connect(const ConnectorPayload& connectorPayload)
{
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(connectorPayload.host),
                                            connectorPayload.port};

    socket->connect(endpoint);

    boost::asio::socket_base::keep_alive option(true);
    
    socket->set_option(option);
}
}