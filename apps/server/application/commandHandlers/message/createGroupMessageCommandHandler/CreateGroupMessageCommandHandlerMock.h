#pragma once

#include <gmock/gmock.h>

#include "CreateGroupMessageCommandHandler.h"

namespace server::application
{
class CreateGroupMessageCommandHandlerMock : public CreateGroupMessageCommandHandler
{
public:
    MOCK_METHOD(CreateGroupMessageCommandHandlerResult, execute, (const CreateGroupMessageCommandHandlerPayload&),
                (const override));
};
}
