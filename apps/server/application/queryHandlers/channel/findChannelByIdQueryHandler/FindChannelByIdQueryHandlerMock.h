#pragma once

#include <gmock/gmock.h>

#include "FindChannelByIdQueryHandler.h"

namespace server::application
{
class FindChannelByIdQueryHandlerMock : public FindChannelByIdQueryHandler
{
public:
    MOCK_METHOD(FindChannelByIdQueryHandlerResult, execute,
                (const FindChannelByIdQueryHandlerPayload&), (const override));
};
}
