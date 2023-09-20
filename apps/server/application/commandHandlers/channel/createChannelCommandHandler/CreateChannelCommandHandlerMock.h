#pragma once

#include <gmock/gmock.h>

#include "CreateChannelCommandHandler.h"

namespace server::application
{
class CreateChannelCommandHandlerMock : public CreateChannelCommandHandler
{
public:
    MOCK_METHOD(CreateChannelCommandHandlerResult, execute, (const CreateChannelCommandHandlerPayload&),
                (const override));
};
}
