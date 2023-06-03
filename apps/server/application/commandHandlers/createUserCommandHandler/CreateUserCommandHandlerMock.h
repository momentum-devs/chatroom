#pragma once

#include <gmock/gmock.h>

#include "CreateUserCommandHandler.h"

namespace server::application
{
class CreateUserCommandHandlerMock : public CreateUserCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const CreateUserCommandHandlerPayload&), ());
};
}
