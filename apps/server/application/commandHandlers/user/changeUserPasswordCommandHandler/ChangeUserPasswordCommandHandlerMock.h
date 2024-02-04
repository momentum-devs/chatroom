#pragma once

#include <gmock/gmock.h>

#include "ChangeUserPasswordCommandHandler.h"

namespace server::application
{
class ChangeUserPasswordCommandHandlerMock : public ChangeUserPasswordCommandHandler
{
public:
    MOCK_METHOD(void, execute, (const ChangeUserPasswordCommandHandlerPayload&), (const override));
};
}
