#pragma once

#include <gmock/gmock.h>

#include "FindUserFriendsQueryHandler.h"

namespace server::application
{
class FindUserFriendsQueryHandlerMock : public FindUserFriendsQueryHandler
{
public:
    MOCK_METHOD(FindUserFriendsQueryHandlerResult, execute,
                (const FindUserFriendsQueryHandlerPayload&), (const override));
};
}
