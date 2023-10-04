#pragma once

#include <gmock/gmock.h>

#include "FindReceivedChannelInvitationsQueryHandler.h"

namespace server::application
{
class FindReceivedChannelInvitationsQueryHandlerMock : public FindReceivedChannelInvitationsQueryHandler
{
public:
    MOCK_METHOD(FindReceivedChannelInvitationsQueryHandlerResult, execute,
                (const FindReceivedChannelInvitationsQueryHandlerPayload&), (const override));
};
}
