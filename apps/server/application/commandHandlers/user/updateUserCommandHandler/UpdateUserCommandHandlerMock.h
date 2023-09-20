#pragma once

#include <gmock/gmock.h>

#include "UpdateUserCommandHandler.h"

namespace server::application
{
class UpdateUserCommandHandlerMock : public UpdateUserCommandHandler
{
public:
    MOCK_METHOD(UpdateUserCommandHandlerResult, execute, (const UpdateUserCommandHandlerPayload&), (const override));
};
}
