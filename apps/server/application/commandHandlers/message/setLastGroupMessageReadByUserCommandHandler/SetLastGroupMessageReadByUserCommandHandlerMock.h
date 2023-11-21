#pragma once

#include <gmock/gmock.h>

#include "SetLastGroupMessageReadByUserCommandHandler.h"

namespace server::application
{
class SetLastGroupMessageReadByUserCommandHandlerMock : public SetLastGroupMessageReadByUserCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const SetLastGroupMessageReadByUserCommandHandlerPayload&), (const override));
};
}
