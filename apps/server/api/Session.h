#pragma once

#include <boost/asio.hpp>

namespace server::api
{
class Session
{
public:
    virtual boost::asio::ip::tcp::socket& getSocket() = 0;
};
}