#pragma once

#include <gmock/gmock.h>

#include "AcceptChannelInvitationCommandHandler.h"

namespace server::application
{
class AcceptChannelInvitationCommandHandlerMock : public AcceptChannelInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const AcceptChannelInvitationCommandHandlerPayload&), (const override));
};
}
