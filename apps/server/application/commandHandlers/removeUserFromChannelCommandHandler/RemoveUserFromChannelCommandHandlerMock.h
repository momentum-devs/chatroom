#pragma once

#include <gmock/gmock.h>

#include "RemoveUserFromChannelCommandHandler.h"

namespace server::application
{
class RemoveUserFromChannelCommandHandlerMock : public RemoveUserFromChannelCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const RemoveUserFromChannelCommandHandlerPayload&), ());
};
}
