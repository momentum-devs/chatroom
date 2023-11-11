#pragma once

#include <gmock/gmock.h>

#include "UpdateMessageCommandHandler.h"

namespace server::application
{
class UpdateMessageCommandHandlerMock : public UpdateMessageCommandHandler
{
public:
    MOCK_METHOD(UpdateMessageCommandHandlerResult, execute, (const UpdateMessageCommandHandlerPayload&), (const override));
};
}
