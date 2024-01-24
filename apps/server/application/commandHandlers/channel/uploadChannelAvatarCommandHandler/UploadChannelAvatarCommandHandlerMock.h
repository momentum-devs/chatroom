#pragma once

#include <gmock/gmock.h>

#include "UploadChannelAvatarCommandHandler.h"

namespace server::application
{
class UploadChannelAvatarCommandHandlerMock : public UploadChannelAvatarCommandHandler
{
public:
    MOCK_METHOD(UploadChannelAvatarCommandHandlerResult, execute, (const UploadChannelAvatarCommandHandlerPayload&), (const override));
};
}
