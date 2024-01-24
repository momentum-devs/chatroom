#pragma once

#include <memory>

#include "server/application/services/hashService/HashService.h"
#include "server/application/services/s3Service/S3Service.h"
#include "server/core/configProvider/ConfigProvider.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "UploadChannelAvatarCommandHandler.h"

namespace server::application
{
class UploadChannelAvatarCommandHandlerImpl : public UploadChannelAvatarCommandHandler
{
public:
    UploadChannelAvatarCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>, std::shared_ptr<S3Service>,
                                       std::shared_ptr<core::ConfigProvider>);

    UploadChannelAvatarCommandHandlerResult execute(const UploadChannelAvatarCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<S3Service> s3Service;
    std::shared_ptr<core::ConfigProvider> configProvider;
};
}
