#pragma once

#include <gmock/gmock.h>

#include "CreateGroupCommandHandler.h"

namespace server::application
{
class CreateGroupCommandHandlerMock : public CreateGroupCommandHandler
{
public:
    MOCK_METHOD(CreateGroupCommandHandlerResult, execute, (const CreateGroupCommandHandlerPayload&), (const override));
};
}
