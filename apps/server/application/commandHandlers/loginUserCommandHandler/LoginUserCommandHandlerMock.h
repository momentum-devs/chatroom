#pragma once

#include <gmock/gmock.h>

#include "LoginUserCommandHandler.h"

namespace server::application
{
class LoginUserCommandHandlerMock : public LoginUserCommandHandler
{
public:
    MOCK_METHOD(LoginUserCommandHandlerResult, execute, (const LoginUserCommandHandlerPayload&), (const override));
};
}
