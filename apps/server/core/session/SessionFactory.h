#pragma once

#include <memory>

#include "Session.h"

namespace server::core
{
class SessionFactory
{
public:
    virtual ~SessionFactory() = default;

    virtual std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> create() const = 0;
};
}
