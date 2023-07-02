#pragma once

#include <gmock/gmock.h>

#include "FindUsersBelongingToChannelQueryHandler.h"

namespace server::application
{
class FindUsersChannelsByChannelIdQueryHandlerMock : public FindUsersChannelsByChannelIdQueryHandler
{
public:
    MOCK_METHOD(FindUsersChannelsByChannelIdQueryHandlerResult, execute,
                (const FindUsersChannelsByChannelIdQueryHandlerPayload&), (const override));
};
}
