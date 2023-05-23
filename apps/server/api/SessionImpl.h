#pragma once

#include "Session.h"

namespace server::api
{
class SessionImpl final: public Session
{
public:
    SessionImpl(boost::asio::io_context& contextInit);
    boost::asio::ip::tcp::socket& getSocket() override;

private:
    boost::asio::io_context& context;
    boost::asio::ip::tcp::socket socket;
};
}