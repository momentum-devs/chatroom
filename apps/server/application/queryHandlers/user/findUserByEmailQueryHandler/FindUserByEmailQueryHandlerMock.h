#pragma once

#include <gmock/gmock.h>

#include "FindUserByEmailQueryHandler.h"

namespace server::application
{
class FindUserByEmailQueryHandlerMock : public FindUserByEmailQueryHandler
{
public:
    MOCK_METHOD(FindUserByEmailQueryHandlerResult, execute, (const FindUserByEmailQueryHandlerPayload&),
                (const override));
};
}
