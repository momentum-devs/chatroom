#pragma once

#include <gmock/gmock.h>

#include "VerifyUserEmailCommandHandler.h"

namespace server::application
{
class VerifyUserEmailCommandHandlerMock : public VerifyUserEmailCommandHandler
{
public:
    MOCK_METHOD(VerifyUserEmailCommandHandlerResult, execute, (const VerifyUserEmailCommandHandlerPayload&),
                (const override));
};
}
