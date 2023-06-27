#pragma once

#include <gmock/gmock.h>

#include "FindUsersChannelsByUserIdQueryHandler.h"

namespace server::application
{
class FindUsersChannelsByUserIdQueryHandlerMock : public FindUsersChannelsByUserIdQueryHandler
{
public:
    MOCK_METHOD(FindUsersChannelsByUserIdQueryHandlerResult, execute,
                (const FindUsersChannelsByUserIdQueryHandlerPayload&), (const override));
};
}
