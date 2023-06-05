#pragma once

#include <gmock/gmock.h>

#include "SessionFactory.h"

namespace server::api
{
class SessionFactoryMock : public SessionFactory
{
public:
    MOCK_METHOD((std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>>), create, (),
                (const override));
};
}
