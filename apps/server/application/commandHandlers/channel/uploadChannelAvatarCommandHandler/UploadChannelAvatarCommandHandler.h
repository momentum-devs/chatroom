#pragma once

#include "payloads/UploadChannelAvatarCommandHandlerPayload.h"
#include "payloads/UploadChannelAvatarCommandHandlerResult.h"

namespace server::application
{
class UploadChannelAvatarCommandHandler
{
public:
    virtual ~UploadChannelAvatarCommandHandler() = default;

    virtual UploadChannelAvatarCommandHandlerResult execute(const UploadChannelAvatarCommandHandlerPayload&) const = 0;
};

}
