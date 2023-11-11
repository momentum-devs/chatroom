#pragma once

#include <gmock/gmock.h>

#include "DeleteMessageCommandHandler.h"

namespace server::application
{
class DeleteMessageCommandHandlerMock : public DeleteMessageCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteMessageCommandHandlerPayload&), ());
};
}
