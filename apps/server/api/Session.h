#pragma once

#include <boost/asio.hpp>

namespace server::api
{
class Session
{
public:
    virtual ~Session() = default;

    virtual void startSession() = 0;
};
}
