#pragma once

#include <gmock/gmock.h>

#include "LeaveChannelCommandHandler.h"

namespace server::application
{
class LeaveChannelCommandHandlerMock : public LeaveChannelCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const LeaveChannelCommandHandlerPayload&), ());
};
}
