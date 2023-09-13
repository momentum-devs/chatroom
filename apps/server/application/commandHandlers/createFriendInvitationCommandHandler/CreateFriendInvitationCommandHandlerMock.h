#pragma once

#include <gmock/gmock.h>

#include "CreateFriendInvitationCommandHandler.h"

namespace server::application
{
class CreateFriendInvitationCommandHandlerMock : public CreateFriendInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const CreateFriendInvitationCommandHandlerPayload&), (const override));
};
}
