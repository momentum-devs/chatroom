#pragma once

#include <gmock/gmock.h>

#include "FindMessageReactionsQueryHandler.h"

namespace server::application
{
class FindMessageReactionsQueryHandlerMock : public FindMessageReactionsQueryHandler
{
public:
    MOCK_METHOD(FindMessageReactionsQueryHandlerResult, execute, (const FindMessageReactionsQueryHandlerPayload&),
                (const override));
};
}
