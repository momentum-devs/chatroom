#include "UploadUserAvatarCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceAlreadyExistsError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UploadUserAvatarCommandHandlerImpl::UploadUserAvatarCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit, std::shared_ptr<HashService> hashServiceInit)
    : userRepository{std::move(userRepositoryInit)}, hashService{std::move(hashServiceInit)}
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

    const auto avatarUrl = "";

    existingUser->get()->setAvatarUrl(avatarUrl);

    const auto user = userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("User avatar uploaded. {{userId: {}}}", payload.id);

    return {avatarUrl};
}
}
