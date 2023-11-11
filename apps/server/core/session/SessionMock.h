#pragma once

#include <gmock/gmock.h>

#include "Session.h"

namespace server::core
{
class SessionMock : public Session
{
public:
    MOCK_METHOD(void, startSession, (), (override));
    MOCK_METHOD(void, sendMessage, (const common::messages::Message& message), (override));
    MOCK_METHOD(bool, isActive, (), (override));
    MOCK_METHOD(void, close, (), (override));
};
}
