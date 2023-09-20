#pragma once

#include <gmock/gmock.h>

#include "CreateFriendshipCommandHandler.h"

namespace server::application
{
class CreateFriendshipCommandHandlerMock : public CreateFriendshipCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const CreateFriendshipCommandHandlerPayload&), (const override));
};
}
