#pragma once

#include <gmock/gmock.h>

#include "FindGroupMessagesQueryHandler.h"

namespace server::application
{
class FindGroupMessagesQueryHandlerMock : public FindGroupMessagesQueryHandler
{
public:
    MOCK_METHOD(FindGroupMessagesQueryHandlerResult, execute,
                (const FindGroupMessagesQueryHandlerPayload&), (const override));
};
}
