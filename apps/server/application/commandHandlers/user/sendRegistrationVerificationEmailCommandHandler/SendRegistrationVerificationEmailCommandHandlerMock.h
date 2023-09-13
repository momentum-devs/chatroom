#pragma once

#include <gmock/gmock.h>

#include "SendRegistrationVerificationEmailCommandHandler.h"

namespace server::application
{
class SendRegistrationVerificationEmailCommandHandlerMock : public SendRegistrationVerificationEmailCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const SendRegistrationVerificationEmailCommandHandlerPayload&), (const override));
};
}
