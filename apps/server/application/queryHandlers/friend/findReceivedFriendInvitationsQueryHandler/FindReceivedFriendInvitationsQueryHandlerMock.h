#pragma once

#include <gmock/gmock.h>

#include "FindReceivedFriendInvitationsQueryHandler.h"

namespace server::application
{
class FindReceivedFriendInvitationsQueryHandlerMock : public FindReceivedFriendInvitationsQueryHandler
{
public:
    MOCK_METHOD(FindReceivedFriendInvitationsQueryHandlerResult, execute,
                (const FindReceivedFriendInvitationsQueryHandlerPayload&), (const override));
};
}
