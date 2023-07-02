#pragma once

#include <gmock/gmock.h>

#include "FindChannelsToWhichUserBelongsQueryHandler.h"

namespace server::application
{
class FindChannelsToWhichUserBelongsQueryHandlerMock : public FindChannelsToWhichUserBelongsQueryHandler
{
public:
    MOCK_METHOD(FindChannelsToWhichUserBelongsQueryHandlerResult, execute,
                (const FindChannelsToWhichUserBelongsQueryHandlerPayload&), (const override));
};
}
