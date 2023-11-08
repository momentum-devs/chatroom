#pragma once

#include <gmock/gmock.h>

#include "FindChannelConversationQueryHandler.h"

namespace server::application
{
class FindChannelConversationQueryHandlerMock : public FindChannelConversationQueryHandler
{
public:
    MOCK_METHOD(FindChannelConversationQueryHandlerResult, execute, (const FindChannelConversationQueryHandlerPayload&),
                (const override));
};
}
