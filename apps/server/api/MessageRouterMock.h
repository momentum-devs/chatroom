#pragma once

#include <gmock/gmock.h>

#include "MessageRouter.h"

namespace server::api
{
class MessageRouterMock : public MessageRouter
{
public:
    MOCK_METHOD(common::messages::Message, route, (const common::messages::Message& message), (override));
};
}
