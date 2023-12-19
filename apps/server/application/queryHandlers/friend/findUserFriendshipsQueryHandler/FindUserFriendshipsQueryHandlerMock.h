#pragma once

#include <gmock/gmock.h>

#include "FindUserFriendshipsQueryHandler.h"

namespace server::application
{
class FindUserFriendshipsQueryHandlerMock : public FindUserFriendshipsQueryHandler
{
public:
    MOCK_METHOD(FindUserFriendshipsQueryHandlerResult, execute, (const FindUserFriendshipsQueryHandlerPayload&),
                (const override));
};
}
