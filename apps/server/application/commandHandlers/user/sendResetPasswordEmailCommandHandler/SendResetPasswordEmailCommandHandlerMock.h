#pragma once

#include <gmock/gmock.h>

#include "SendResetPasswordEmailCommandHandler.h"

namespace server::application
{
class SendResetPasswordEmailCommandHandlerMock : public SendResetPasswordEmailCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const SendResetPasswordEmailCommandHandlerPayload&), (const override));
};
}
