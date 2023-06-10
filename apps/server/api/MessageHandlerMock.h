#pragma once

#include <gmock/gmock.h>

#include "MessageHandler.h"

namespace server::api
{
class MessageHandlerMock : public MessageHandler
{
public:
    MOCK_METHOD(common::messages::Message, handleMessage, (const common::messages::Message& message), (override));
};
}
