#pragma once

#include <gmock/gmock.h>

#include "SessionFactory.h"

namespace server::api
{
class SessionFactoryMock : public SessionFactory
{
public:
    MOCK_METHOD(std::shared_ptr<Session>, create, (), (const override));
};
}
