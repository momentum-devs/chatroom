#pragma once

#include <gmock/gmock.h>

#include "CreateChannelInvitationCommandHandler.h"

namespace server::application
{
class CreateChannelInvitationCommandHandlerMock : public CreateChannelInvitationCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const CreateChannelInvitationCommandHandlerPayload&), (const override));
};
}
