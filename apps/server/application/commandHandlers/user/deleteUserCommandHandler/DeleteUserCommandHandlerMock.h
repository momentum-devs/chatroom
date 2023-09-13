#pragma once

#include <gmock/gmock.h>

#include "DeleteUserCommandHandler.h"

namespace server::application
{
class DeleteUserCommandHandlerMock : public DeleteUserCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteUserCommandHandlerPayload&), ());
};
}
