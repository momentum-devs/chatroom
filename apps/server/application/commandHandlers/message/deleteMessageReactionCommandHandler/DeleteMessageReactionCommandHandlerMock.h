#pragma once

#include <gmock/gmock.h>

#include "DeleteMessageReactionCommandHandler.h"

namespace server::application
{
class DeleteMessageReactionCommandHandlerMock : public DeleteMessageReactionCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteMessageReactionCommandHandlerPayload&), (const override));
};
}
