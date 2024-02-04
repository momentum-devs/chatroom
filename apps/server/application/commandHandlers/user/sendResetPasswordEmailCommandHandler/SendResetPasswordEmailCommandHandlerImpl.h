#pragma once

#include <memory>

#include "SendResetPasswordEmailCommandHandler.h"
#include "server/application/services/emailService/EmailService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class SendResetPasswordEmailCommandHandlerImpl : public SendResetPasswordEmailCommandHandler
{
public:
    SendResetPasswordEmailCommandHandlerImpl(std::shared_ptr<domain::UserRepository>,
                                                        std::shared_ptr<application::EmailService>);

    void execute(const SendResetPasswordEmailCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<application::EmailService> emailService;
};
}
