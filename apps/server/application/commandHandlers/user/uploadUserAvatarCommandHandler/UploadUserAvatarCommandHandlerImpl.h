#pragma once

#include <memory>

#include "server/application/services/hashService/HashService.h"
#include "server/application/services/s3Service/S3Service.h"
#include "server/core/configProvider/ConfigProvider.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "UploadUserAvatarCommandHandler.h"

namespace server::application
{
class UploadUserAvatarCommandHandlerImpl : public UploadUserAvatarCommandHandler
{
public:
    UploadUserAvatarCommandHandlerImpl(std::shared_ptr<domain::UserRepository>, std::shared_ptr<S3Service>,
                                       std::shared_ptr<core::ConfigProvider>);

    UploadUserAvatarCommandHandlerResult execute(const UploadUserAvatarCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<S3Service> s3Service;
    std::shared_ptr<core::ConfigProvider> configProvider;
};
}
