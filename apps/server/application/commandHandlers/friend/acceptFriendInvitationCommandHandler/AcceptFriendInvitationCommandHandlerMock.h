#pragma once

#include <gmock/gmock.h>

#include "AcceptFriendInvitationCommandHandler.h"

namespace server::application
{
class AcceptFriendInvitationCommandHandlerMock : public AcceptFriendInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const AcceptFriendInvitationCommandHandlerPayload&), (const override));
};
}
