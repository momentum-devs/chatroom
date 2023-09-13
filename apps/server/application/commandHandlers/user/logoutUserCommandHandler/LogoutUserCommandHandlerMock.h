#pragma once

#include <gmock/gmock.h>

#include "LogoutUserCommandHandler.h"

namespace server::application
{
class LogoutUserCommandHandlerMock : public LogoutUserCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const LogoutUserCommandHandlerPayload&), (const override));
};
}
