#include "VerifyUserEmailCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

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
    LOG_S(INFO) << fmt::format("Verifying user email \"{}\"...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with email \"{}\" not found.", payload.email)};
    }

    if (existingUser->get()->isEmailVerified())
    {
        return {true};
    }

    if (existingUser->get()->getVerificationCode() != payload.verificationCode)
    {
        LOG_S(INFO) << fmt::format("User with email \"{}\" not verified.", payload.email);

        return {false};
    }

    existingUser->get()->setEmailVerified(true);

    userRepository->updateUser({**existingUser});

    LOG_S(INFO) << fmt::format("User with email \"{}\" verified.", payload.email);

    return {true};
}

}
