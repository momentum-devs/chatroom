#pragma once

#include <gmock/gmock.h>

#include "FindChannelMessagesQueryHandler.h"

namespace server::application
{
class FindChannelMessagesQueryHandlerMock : public FindChannelMessagesQueryHandler
{
public:
    MOCK_METHOD(FindChannelMessagesQueryHandlerResult, execute, (const FindChannelMessagesQueryHandlerPayload&),
                (const override));
};
}
