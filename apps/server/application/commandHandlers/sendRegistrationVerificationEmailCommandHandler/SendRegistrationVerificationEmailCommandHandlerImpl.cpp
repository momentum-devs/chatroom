#include "SendRegistrationVerificationEmailCommandHandlerImpl.h"

#include <format>

#include "../../errors/ResourceNotFoundError.h"
#include "loguru.hpp"

namespace server::application
{
SendRegistrationVerificationEmailCommandHandlerImpl::SendRegistrationVerificationEmailCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<application::EmailService> emailServiceInit)
    : userRepository{std::move(userRepositoryInit)}, emailService{std::move(emailServiceInit)}
{
}

void SendRegistrationVerificationEmailCommandHandlerImpl::execute(
    const SendRegistrationVerificationEmailCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format("Sending registration verification email to \"{}\"...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{std::format("User with email \"{}\" not found.", payload.email)};
    }

    LOG_S(INFO) << std::format("Registration verification email sent to \"{}\".", payload.email);
}

}
