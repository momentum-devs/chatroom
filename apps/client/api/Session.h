#pragma once

#include <boost/asio.hpp>
#include <string>

namespace client::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void connect(const std::string& hostName, unsigned short portNumber) = 0;
};
}