#pragma once

#include <gmock/gmock.h>

#include "AddUserToGroupCommandHandler.h"

namespace server::application
{
class AddUserToGroupCommandHandlerMock : public AddUserToGroupCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const AddUserToGroupCommandHandlerPayload&), (const override));
};
}
