#pragma once

#include <gmock/gmock.h>

#include "FindUsersChannelsByChannelIdQueryHandler.h"

namespace server::application
{
class FindUsersChannelsByChannelIdQueryHandlerMock : public FindUsersChannelsByChannelIdQueryHandler
{
public:
    MOCK_METHOD(FindUsersChannelsByChannelIdQueryHandlerResult, execute,
                (const FindUsersChannelsByChannelIdQueryHandlerPayload&), (const override));
};
}
