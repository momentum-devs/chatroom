#pragma once

#include <gmock/gmock.h>

#include "CreateChannelMessageCommandHandler.h"

namespace server::application
{
class CreateChannelMessageCommandHandlerMock : public CreateChannelMessageCommandHandler
{
public:
    MOCK_METHOD(CreateChannelMessageCommandHandlerResult, execute, (const CreateChannelMessageCommandHandlerPayload&),
                (const override));
};
}
