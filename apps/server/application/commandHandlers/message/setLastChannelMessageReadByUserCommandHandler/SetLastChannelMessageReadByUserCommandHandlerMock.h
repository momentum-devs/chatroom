#pragma once

#include <gmock/gmock.h>

#include "SetLastChannelMessageReadByUserCommandHandler.h"

namespace server::application
{
class SetLastChannelMessageReadByUserCommandHandlerMock : public SetLastChannelMessageReadByUserCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const SetLastChannelMessageReadByUserCommandHandlerPayload&), (const override));
};
}
