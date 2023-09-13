#pragma once

#include <gmock/gmock.h>

#include "RejectFriendInvitationCommandHandler.h"

namespace server::application
{
class RejectFriendInvitationCommandHandlerMock : public RejectFriendInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const RejectFriendInvitationCommandHandlerPayload&), (const override));
};
}
