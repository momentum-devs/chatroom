#pragma once

#include <gmock/gmock.h>

#include "DeleteChannelCommandHandler.h"

namespace server::application
{
class DeleteChannelCommandHandlerMock : public DeleteChannelCommandHandler
{
public:
    MOCK_METHOD(DeleteChannelCommandHandlerResult, execute, (const DeleteChannelCommandHandlerPayload&),
                (const override));
};
}
