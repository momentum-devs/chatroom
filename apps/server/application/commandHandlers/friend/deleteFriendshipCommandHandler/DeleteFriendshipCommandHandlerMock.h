#pragma once

#include <gmock/gmock.h>

#include "DeleteFriendshipCommandHandler.h"

namespace server::application
{
class DeleteFriendshipCommandHandlerMock : public DeleteFriendshipCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const DeleteFriendshipCommandHandlerPayload&), (const override));
};
}
