#pragma once

#include "payloads/UploadUserAvatarCommandHandlerPayload.h"
#include "payloads/UploadUserAvatarCommandHandlerResult.h"

namespace server::application
{
class UploadUserAvatarCommandHandler
{
public:
    virtual ~UploadUserAvatarCommandHandler() = default;

    virtual UploadUserAvatarCommandHandlerResult execute(const UploadUserAvatarCommandHandlerPayload&) const = 0;
};

}
