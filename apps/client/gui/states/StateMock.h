#pragma once

#include <gmock/gmock.h>

#include "State.h"

namespace client::gui
{
class StateMock : public State
{
public:
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
};
}
