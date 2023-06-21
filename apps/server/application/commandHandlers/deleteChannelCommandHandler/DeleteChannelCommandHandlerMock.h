#pragma once

#include <gmock/gmock.h>

#include "DeleteChannelCommandHandler.h"

namespace server::application
{
class DeleteChannelCommandHandlerMock : public DeleteChannelCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteChannelCommandHandlerPayload&), ());
};
}
