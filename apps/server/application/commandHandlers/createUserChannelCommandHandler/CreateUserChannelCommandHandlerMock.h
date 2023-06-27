#pragma once

#include <gmock/gmock.h>

#include "CreateUserChannelCommandHandler.h"

namespace server::application
{
class CreateUserChannelCommandHandlerMock : public CreateUserChannelCommandHandler
{
public:
    MOCK_METHOD(CreateUserChannelCommandHandlerResult, execute, (const CreateUserChannelCommandHandlerPayload&),
                (const override));
};
}
