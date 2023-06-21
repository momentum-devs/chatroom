#pragma once

#include <gmock/gmock.h>

#include "RegisterUserCommandHandler.h"

namespace server::application
{
class RegisterUserCommandHandlerMock : public RegisterUserCommandHandler
{
public:
    MOCK_METHOD(RegisterUserCommandHandlerResult, execute, (const RegisterUserCommandHandlerPayload&), (const override));
};
}
