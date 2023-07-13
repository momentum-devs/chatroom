#include "SendRegistrationVerificationEmailCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"
#include "loguru.hpp"

namespace server::application
{
VerifyUserEmailCommandHandlerImpl::VerifyUserEmailCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit)
    : userRepository{std::move(userRepositoryInit)}
{
}

VerifyUserEmailCommandHandlerResult
VerifyUserEmailCommandHandlerImpl::execute(const VerifyUserEmailCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Verifying user email \"{}\"...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    if (existingUser->get()->isEmailVerified())
    {
        return {true};
    }

    if (existingUser->get()->getVerificationCode() != payload.verificationCode)
    {
        LOG_S(INFO) << std::format("User with email \"{}\" not verified.", payload.email);

        return {false};
    }

    existingUser->get()->setEmailVerified(true);

    userRepository->updateUser({**existingUser});

    LOG_S(INFO) << std::format("User with email \"{}\" verified.", payload.email);

    return {true};
}

}
