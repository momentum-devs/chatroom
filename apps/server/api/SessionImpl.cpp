#include "SessionImpl.h"

namespace server::api
{
SessionImpl::SessionImpl(boost::asio::io_context& contextInit) : context{contextInit}, socket{context} {}

boost::asio::ip::tcp::socket& SessionImpl::getSocket()
{
    return socket;
}
}