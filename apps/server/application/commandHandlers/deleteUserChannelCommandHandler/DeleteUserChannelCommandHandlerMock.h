#pragma once

#include <gmock/gmock.h>

#include "DeleteUserChannelCommandHandler.h"

namespace server::application
{
class DeleteUserChannelCommandHandlerMock : public DeleteUserChannelCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteUserChannelCommandHandlerPayload&), ());
};
}
