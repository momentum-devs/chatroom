#pragma once

#include <memory>

#include "SendRegistrationVerificationEmailCommandHandler.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class SendRegistrationVerificationEmailCommandHandlerImpl : public SendRegistrationVerificationEmailCommandHandler
{
public:
    SendRegistrationVerificationEmailCommandHandlerImpl(std::shared_ptr<domain::UserRepository>);

    void execute(const SendRegistrationVerificationEmailCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
