#pragma once

#include <gmock/gmock.h>

#include "FindUsersBelongingToGroupQueryHandler.h"

namespace server::application
{
class FindUsersBelongingToGroupQueryHandlerMock : public FindUsersBelongingToGroupQueryHandler
{
public:
    MOCK_METHOD(FindUsersBelongingToGroupQueryHandlerResult, execute,
                (const FindUsersBelongingToGroupQueryHandlerPayload&), (const override));
};
}
