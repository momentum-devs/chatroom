#include "UploadUserAvatarCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UploadUserAvatarCommandHandlerImpl::UploadUserAvatarCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit, std::shared_ptr<S3Service> s3ServiceInit,
    std::shared_ptr<core::ConfigProvider> configProviderInit)
    : userRepository{std::move(userRepositoryInit)},
      s3Service{std::move(s3ServiceInit)},
      configProvider{std::move(configProviderInit)}
{
}

UploadUserAvatarCommandHandlerResult
UploadUserAvatarCommandHandlerImpl::execute(const UploadUserAvatarCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Uploading user avatar... {{userId: {}}}", payload.id);

    auto existingUser = userRepository->findUserById({payload.id});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id \"{}\" not found.", payload.id)};
    }

    const auto bucketName = configProvider->getAwsBucket();

    const auto objectKey = fmt::format("{}{}", payload.id, payload.avatarName);

    s3Service->putObject({bucketName, objectKey, payload.avatarData});

    const auto avatarUrl = fmt::format("https://{}.s3-{}.amazonaws.com/avatars/{}", configProvider->getAwsBucket(),
                                       configProvider->getAwsRegion(), objectKey);

    existingUser->get()->setAvatarUrl(avatarUrl);

    const auto user = userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("User avatar uploaded. {{userId: {}}}", payload.id);

    return {avatarUrl};
}
}
