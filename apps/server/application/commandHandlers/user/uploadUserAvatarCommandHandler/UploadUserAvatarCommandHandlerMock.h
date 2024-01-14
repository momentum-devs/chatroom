#pragma once

#include <gmock/gmock.h>

#include "UploadUserAvatarCommandHandler.h"

namespace server::application
{
class UploadUserAvatarCommandHandlerMock : public UploadUserAvatarCommandHandler
{
public:
    MOCK_METHOD(UploadUserAvatarCommandHandlerResult, execute, (const UploadUserAvatarCommandHandlerPayload&), (const override));
};
}
