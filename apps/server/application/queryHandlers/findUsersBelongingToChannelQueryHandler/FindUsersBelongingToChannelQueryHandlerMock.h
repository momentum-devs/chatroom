#pragma once

#include <gmock/gmock.h>

#include "FindUsersBelongingToChannelQueryHandler.h"

namespace server::application
{
class FindUsersBelongingToChannelQueryHandlerMock : public FindUsersBelongingToChannelQueryHandler
{
public:
    MOCK_METHOD(FindUsersBelongingToChannelQueryHandlerResult, execute,
                (const FindUsersBelongingToChannelQueryHandlerPayload&), (const override));
};
}
