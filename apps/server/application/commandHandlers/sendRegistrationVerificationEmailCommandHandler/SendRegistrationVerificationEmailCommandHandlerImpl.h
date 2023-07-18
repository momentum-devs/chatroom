#pragma once

#include <memory>

#include "SendRegistrationVerificationEmailCommandHandler.h"
#include "server/application/services/emailService/EmailService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class SendRegistrationVerificationEmailCommandHandlerImpl : public SendRegistrationVerificationEmailCommandHandler
{
public:
    SendRegistrationVerificationEmailCommandHandlerImpl(std::shared_ptr<domain::UserRepository>,
                                                        std::shared_ptr<application::EmailService>);

    void execute(const SendRegistrationVerificationEmailCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<application::EmailService> emailService;
};
}
