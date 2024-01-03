#pragma once

#include <gmock/gmock.h>

#include "UpdateChannelCommandHandler.h"

namespace server::application
{
class UpdateChannelCommandHandlerMock : public UpdateChannelCommandHandler
{
public:
    MOCK_METHOD(UpdateChannelCommandHandlerResult, execute, (const UpdateChannelCommandHandlerPayload&),
                (const override));
};
}
