#pragma once

#include <gmock/gmock.h>

#include "FindUserQueryHandler.h"

namespace server::application
{
class FindUserQueryHandlerMock : public FindUserQueryHandler
{
public:
    MOCK_METHOD(FindUserQueryHandlerResult, execute, (const FindUserQueryHandlerPayload&), (const override));
};
}
