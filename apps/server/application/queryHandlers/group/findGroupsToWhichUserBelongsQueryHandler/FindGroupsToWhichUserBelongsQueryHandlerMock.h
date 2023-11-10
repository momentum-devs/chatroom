#pragma once

#include <gmock/gmock.h>

#include "FindGroupsToWhichUserBelongsQueryHandler.h"

namespace server::application
{
class FindGroupsToWhichUserBelongsQueryHandlerMock : public FindGroupsToWhichUserBelongsQueryHandler
{
public:
    MOCK_METHOD(FindGroupsToWhichUserBelongsQueryHandlerResult, execute,
                (const FindGroupsToWhichUserBelongsQueryHandlerPayload&), (const override));
};
}
