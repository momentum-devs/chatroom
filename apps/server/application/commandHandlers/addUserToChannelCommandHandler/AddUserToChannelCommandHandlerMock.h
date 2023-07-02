#pragma once

#include <gmock/gmock.h>

#include "AddUserToChannelCommandHandler.h"

namespace server::application
{
class AddUserToChannelCommandHandlerMock : public AddUserToChannelCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const AddUserToChannelCommandHandlerPayload&), (const override));
};
}
