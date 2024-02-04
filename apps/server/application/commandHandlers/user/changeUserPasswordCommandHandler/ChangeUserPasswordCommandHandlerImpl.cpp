#include "ChangeUserPasswordCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
ChangeUserPasswordCommandHandlerImpl::ChangeUserPasswordCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit, std::shared_ptr<HashService> hashServiceInit,
    std::shared_ptr<PasswordValidationService> passwordValidationServiceInit)
    : userRepository{std::move(userRepositoryInit)},
      hashService{std::move(hashServiceInit)},
      passwordValidationService{std::move(passwordValidationServiceInit)}
{
}

void ChangeUserPasswordCommandHandlerImpl::execute(const ChangeUserPasswordCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Changing user password \"{}\"...", payload.userId);

    const auto existingUser = userRepository->findUserById({payload.userId});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id \"{}\" not found.", payload.userId)};
    }

    if (existingUser->get()->getResetPasswordCode() != payload.resetPasswordCode)
    {
        throw errors::OperationNotValidError{"Invalid reset password code."};
    }

    if (payload.oldPassword == payload.newPassword)
    {
        throw errors::OperationNotValidError{"Old and new password are the same."};
    }

    passwordValidationService->validate(payload.newPassword);

    const auto hashedNewPassword = hashService->hash(payload.newPassword);

    const auto hashedOldPassword = hashService->hash(payload.oldPassword);

    if (existingUser->get()->getPassword() != hashedOldPassword)
    {
        throw errors::OperationNotValidError{"Invalid old password."};
    }

    existingUser->get()->setPassword(hashedNewPassword);

    userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("Reset password for user with with id \"{}\".", payload.userId);
}

}
