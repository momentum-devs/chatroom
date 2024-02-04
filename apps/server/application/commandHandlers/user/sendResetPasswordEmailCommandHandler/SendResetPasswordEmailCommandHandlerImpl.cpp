#include "SendResetPasswordEmailCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "random/StringGenerator.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
SendResetPasswordEmailCommandHandlerImpl::SendResetPasswordEmailCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<application::EmailService> emailServiceInit)
    : userRepository{std::move(userRepositoryInit)}, emailService{std::move(emailServiceInit)}
{
}

void SendResetPasswordEmailCommandHandlerImpl::execute(const SendResetPasswordEmailCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format("Sending reset password email to \"{}\"...", payload.email);

    const auto existingUser = userRepository->findUserByEmail({payload.email});

    if (!existingUser)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with email \"{}\" not found.", payload.email)};
    }

    const auto resetPasswordCode = common::random::StringGenerator::numeric(6);

    existingUser->get()->setResetPasswordCode(resetPasswordCode);

    emailService->sendEmail({payload.email, "Chatroom Reset Password", resetPasswordCode});

    LOG_S(INFO) << fmt::format("Rest password email sent to \"{}\".", payload.email);
}

}
