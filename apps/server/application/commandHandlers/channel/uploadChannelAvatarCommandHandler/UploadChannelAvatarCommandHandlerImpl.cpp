#include "UploadChannelAvatarCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UploadChannelAvatarCommandHandlerImpl::UploadChannelAvatarCommandHandlerImpl(
    std::shared_ptr<domain::ChannelRepository> channelRepositoryInit, std::shared_ptr<S3Service> s3ServiceInit,
    std::shared_ptr<core::ConfigProvider> configProviderInit)
    : channelRepository{std::move(channelRepositoryInit)},
      s3Service{std::move(s3ServiceInit)},
      configProvider{std::move(configProviderInit)}
{
}

UploadChannelAvatarCommandHandlerResult
UploadChannelAvatarCommandHandlerImpl::execute(const UploadChannelAvatarCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Uploading channel avatar... {{channelId: {}}}", payload.id);

    auto existingChannel = channelRepository->findChannelById({payload.id});

    if (!existingChannel)
    {
        throw errors::ResourceNotFoundError{fmt::format("Channel with id \"{}\" not found.", payload.id)};
    }

    const auto bucketName = configProvider->getAwsBucket();

    const auto objectKey = payload.id;

    s3Service->putObject({bucketName, objectKey, payload.avatarData});

    const auto avatarUrl = fmt::format("https://{}.s3-{}.amazonaws.com/avatars/{}", configProvider->getAwsBucket(),
                                       configProvider->getAwsRegion(), objectKey);

    existingChannel->get()->setAvatarUrl(avatarUrl);

    const auto channel = channelRepository->updateChannel({**existingChannel});

    LOG_S(INFO) << fmt::format("Channel avatar uploaded. {{channelId: {}}}", payload.id);

    return {avatarUrl};
}
}
