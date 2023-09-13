#pragma once

#include <gmock/gmock.h>

#include "RejectChannelInvitationCommandHandler.h"

namespace server::application
{
class RejectChannelInvitationCommandHandlerMock : public RejectChannelInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const RejectChannelInvitationCommandHandlerPayload&), (const override));
};
}
